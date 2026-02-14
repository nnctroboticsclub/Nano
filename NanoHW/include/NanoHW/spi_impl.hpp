#pragma once

#include "spi.hpp"

namespace nano_hw::spi {

// Friend-Injection 用の Impl 関数宣言
void* AllocSPIInterfaceImpl(Pin miso, Pin mosi, Pin sclk, int frequency,
                            ICallbacks* callbacks, void* callback_context);
void FreeSPIInterfaceImpl(void* inst);
void SetModeSPIImpl(void* inst, SPIFormat format);
void SetFrequencySPIImpl(void* inst, int frequency);
int TransferSPIImpl(void* inst, std::vector<uint8_t> const& tx_buffer,
                    std::vector<uint8_t>& rx_buffer);

/// @brief SPI conceptを満たす型から動的ディスパッチ関数を生成
/// @tparam SPIT SPI conceptを満たすテンプレートクラス
template <template <SPIConfig> typename SPIT>
requires SPI<SPIT> class SPIImpl {
  // Config内のOnTransferからコールバックを呼び出すためのConfig
  struct CallbackConfig {
    struct OnTransfer {
      static void execute(void* context, std::vector<uint8_t> const& tx,
                          std::vector<uint8_t> const& rx) {
        auto* ctx = static_cast<std::pair<ICallbacks*, void*>*>(context);
        if (ctx->first != nullptr) {
          ctx->first->OnTransfer(ctx->second, tx, rx);
        }
      }
    };
  };

  using ImplType = SPIT<CallbackConfig>;

  friend void* AllocSPIInterfaceImpl(Pin miso, Pin mosi, Pin sclk,
                                     int frequency, ICallbacks* callbacks,
                                     void* callback_context) {
    auto* impl = new ImplType(miso, mosi, sclk, frequency);
    auto* ctx = new std::pair<ICallbacks*, void*>(callbacks, callback_context);
    // implとコンテキストをペアで保存
    return new std::pair<ImplType*, std::pair<ICallbacks*, void*>*>(impl, ctx);
  }

  friend void FreeSPIInterfaceImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    delete pair->first;
    delete pair->second;
    delete pair;
  }

  friend void SetModeSPIImpl(void* inst, SPIFormat format) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->SetMode(format);
  }

  friend void SetFrequencySPIImpl(void* inst, int frequency) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->SetFrequency(frequency);
  }

  friend int TransferSPIImpl(void* inst, std::vector<uint8_t> const& tx_buffer,
                             std::vector<uint8_t>& rx_buffer) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    int result = pair->first->Transfer(tx_buffer, rx_buffer);
    // コールバックを呼び出す
    CallbackConfig::OnTransfer::execute(pair->second, tx_buffer, rx_buffer);
    return result;
  }
};

// Friend-Injection で定義された関数はオブジェクトファイルに含まれないため、
// 通常の関数を挟んで実体を残す
void* AllocInterface(Pin miso, Pin mosi, Pin sclk, int frequency,
                     ICallbacks* callbacks, void* callback_context) {
  return AllocSPIInterfaceImpl(miso, mosi, sclk, frequency, callbacks,
                               callback_context);
}
void FreeInterface(void* inst) {
  FreeSPIInterfaceImpl(inst);
}
void SetModeImpl(void* inst, SPIFormat format) {
  SetModeSPIImpl(inst, format);
}
void SetFrequencyImpl(void* inst, int frequency) {
  SetFrequencySPIImpl(inst, frequency);
}
int TransferImpl(void* inst, std::vector<uint8_t> const& tx_buffer,
                 std::vector<uint8_t>& rx_buffer) {
  return TransferSPIImpl(inst, tx_buffer, rx_buffer);
}

}  // namespace nano_hw::spi
