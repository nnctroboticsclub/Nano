#pragma once

#include <algorithm>
#include <vector>

#include <NanoHW/spi.hpp>

#include "pin_name.hpp"

namespace mbed {

class SPI {
 public:
  SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel = NC)
      : dri_(ToPin(miso), ToPin(mosi), ToPin(sclk), kDefaultFrequency) {
    (void)ssel;
  }

  void frequency(int hz) { dri_.SetFrequency(hz); }

  void format(int bits, int mode = 0) {
    (void)bits;
    dri_.SetMode(FormatFromMode(mode));
  }

  int write(int value) {
    std::vector<uint8_t> tx{static_cast<uint8_t>(value)};
    std::vector<uint8_t> rx;
    (void)dri_.Transfer(tx, rx);
    return rx.empty() ? 0 : rx.front();
  }

  int write(const char* tx_buffer, int tx_length, char* rx_buffer,
            int rx_length) {
    if (tx_length <= 0 || tx_buffer == nullptr) {
      return 0;
    }

    std::vector<uint8_t> tx(static_cast<size_t>(tx_length));
    for (int i = 0; i < tx_length; ++i) {
      tx[static_cast<size_t>(i)] = static_cast<uint8_t>(tx_buffer[i]);
    }

    std::vector<uint8_t> rx;
    const int transferred = dri_.Transfer(tx, rx);

    if (rx_buffer != nullptr && rx_length > 0) {
      const int copy_length = std::min(rx_length, static_cast<int>(rx.size()));
      for (int i = 0; i < copy_length; ++i) {
        rx_buffer[i] = static_cast<char>(rx[static_cast<size_t>(i)]);
      }
    }

    return transferred;
  }

 private:
  static constexpr int kDefaultFrequency = 1000000;

  static nano_hw::spi::SPIFormat FormatFromMode(int mode) {
    switch (mode) {
      case 1:
        return nano_hw::spi::SPIFormat::Mode1;
      case 2:
        return nano_hw::spi::SPIFormat::Mode2;
      case 3:
        return nano_hw::spi::SPIFormat::Mode3;
      default:
        return nano_hw::spi::SPIFormat::Mode0;
    }
  }

  nano_hw::spi::DynSPI<nano_hw::spi::DummySPIConfig> dri_;
};

}  // namespace mbed
