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

/// @brief UART の実装クラスから動的ディスパッチ関数を生成
/// @tparam Impl UART の実装クラス (コールバックを持つ通常のクラス)
template <typename Impl>
class UARTImpl {
  friend void* AllocUARTInterfaceImpl(Pin transmit_pin, Pin receive_pin,
                                      int frequency, ICallbacks* callbacks,
                                      void* callback_context) {
    return new Impl(transmit_pin, receive_pin, frequency, callbacks,
                    callback_context);
  }

  friend void FreeUARTInterfaceImpl(void* inst) {
    delete static_cast<Impl*>(inst);
  }

  friend void RebaudUARTImpl(void* inst, int frequency) {
    static_cast<Impl*>(inst)->Rebaud(frequency);
  }

  friend size_t SendUARTImpl(void* inst, void* buffer, size_t size) {
    return static_cast<Impl*>(inst)->Send(buffer, size);
  }

  friend size_t ReceiveUARTImpl(void* inst, void* buffer, size_t size) {
    return static_cast<Impl*>(inst)->Receive(buffer, size);
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

}  // namespace nano_hw::uart
