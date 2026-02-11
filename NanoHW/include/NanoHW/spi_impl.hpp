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

/// @brief SPI の実装クラスから動的ディスパッチ関数を生成
/// @tparam Impl SPI の実装クラス (コールバックを持つ通常のクラス)
template <typename Impl>
class SPIImpl {
  friend void* AllocSPIInterfaceImpl(Pin miso, Pin mosi, Pin sclk,
                                     int frequency, ICallbacks* callbacks,
                                     void* callback_context) {
    return new Impl(miso, mosi, sclk, frequency, callbacks, callback_context);
  }

  friend void FreeSPIInterfaceImpl(void* inst) {
    delete static_cast<Impl*>(inst);
  }

  friend void SetModeSPIImpl(void* inst, SPIFormat format) {
    static_cast<Impl*>(inst)->SetMode(format);
  }

  friend void SetFrequencySPIImpl(void* inst, int frequency) {
    static_cast<Impl*>(inst)->SetFrequency(frequency);
  }

  friend int TransferSPIImpl(void* inst, std::vector<uint8_t> const& tx_buffer,
                             std::vector<uint8_t>& rx_buffer) {
    return static_cast<Impl*>(inst)->Transfer(tx_buffer, rx_buffer);
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
