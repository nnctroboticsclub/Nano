#pragma once

#include "uart.hpp"

namespace nano_hw::uart {

// Friend-Injection 用の Impl 関数宣言
void* AllocUARTInterfaceImpl(Pin transmit_pin, Pin receive_pin, int frequency,
                             ICallbacks* callbacks, void* callback_context);
void FreeUARTInterfaceImpl(void* inst);
void RebaudUARTImpl(void* inst, int frequency);
size_t SendUARTImpl(void* inst, void* buffer, size_t size);
size_t ReceiveUARTImpl(void* inst, void* buffer, size_t size);
void FormatUARTImpl(void* inst, int data_bits, Parity parity, int stop_bits);

/// @brief UART conceptを満たす型から動的ディスパッチ関数を生成
/// @tparam UartT UART conceptを満たすテンプレートクラス
template <template <UARTConfig> typename UartT>
requires UART<UartT> class UARTImpl {
  // Config内のコールバックを呼び出すためのConfig
  struct CallbackConfig {
    struct OnUARTRx {
      static void execute(void* context, const uint8_t* buffer, size_t size) {
        auto* ctx = static_cast<std::pair<ICallbacks*, void*>*>(context);
        if (ctx->first != nullptr) {
          ctx->first->OnUARTRx(ctx->second, buffer, size);
        }
      }
    };
    struct OnUARTTx {
      static void execute(void* context, const uint8_t* buffer, size_t size) {
        auto* ctx = static_cast<std::pair<ICallbacks*, void*>*>(context);
        if (ctx->first != nullptr) {
          ctx->first->OnUARTTx(ctx->second, buffer, size);
        }
      }
    };
  };

  using ImplType = UartT<CallbackConfig>;

  friend void* AllocUARTInterfaceImpl(Pin transmit_pin, Pin receive_pin,
                                      int frequency, ICallbacks* callbacks,
                                      void* callback_context) {
    auto* impl = new ImplType(transmit_pin, receive_pin, frequency);
    auto* ctx = new std::pair<ICallbacks*, void*>(callbacks, callback_context);
    return new std::pair<ImplType*, std::pair<ICallbacks*, void*>*>(impl, ctx);
  }

  friend void FreeUARTInterfaceImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    delete pair->first;
    delete pair->second;
    delete pair;
  }

  friend void RebaudUARTImpl(void* inst, int frequency) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->Rebaud(frequency);
  }

  friend size_t SendUARTImpl(void* inst, void* buffer, size_t size) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    size_t result = pair->first->Send(buffer, size);
    // コールバックを呼び出す
    CallbackConfig::OnUARTTx::execute(
        pair->second, static_cast<const uint8_t*>(buffer), size);
    return result;
  }

  friend size_t ReceiveUARTImpl(void* inst, void* buffer, size_t size) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    size_t result = pair->first->Receive(buffer, size);
    // コールバックを呼び出す
    CallbackConfig::OnUARTRx::execute(
        pair->second, static_cast<const uint8_t*>(buffer), size);
    return result;
  }

  friend void FormatUARTImpl(void* inst, int data_bits, Parity parity,
                             int stop_bits) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->Format(data_bits, parity, stop_bits);
  }
};

// Friend-Injection で定義された関数はオブジェクトファイルに含まれないため、
// 通常の関数を挟んで実体を残す
void* AllocInterface(Pin transmit_pin, Pin receive_pin, int frequency,
                     ICallbacks* callbacks, void* callback_context) {
  return AllocUARTInterfaceImpl(transmit_pin, receive_pin, frequency, callbacks,
                                callback_context);
}
void FreeInterface(void* inst) {
  FreeUARTInterfaceImpl(inst);
}
void RebaudImpl(void* inst, int frequency) {
  RebaudUARTImpl(inst, frequency);
}
size_t SendImpl(void* inst, void* buffer, size_t size) {
  return SendUARTImpl(inst, buffer, size);
}
size_t ReceiveImpl(void* inst, void* buffer, size_t size) {
  return ReceiveUARTImpl(inst, buffer, size);
}

void FormatImpl(void* inst, int data_bits, Parity parity, int stop_bits) {
  FormatUARTImpl(inst, data_bits, parity, stop_bits);
}

}  // namespace nano_hw::uart
