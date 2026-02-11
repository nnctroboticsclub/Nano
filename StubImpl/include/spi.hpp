#pragma once
#include <NanoHW/pin.hpp>
#include <NanoHW/spi_impl.hpp>

#include <iostream>
#include <vector>

namespace nano_stub {
using nano_hw::spi::ICallbacks;
using nano_hw::spi::SPIFormat;

namespace {
const char* ToModeName(SPIFormat format) {
  switch (format) {
    case SPIFormat::Mode1:
      return "Mode1";
    case SPIFormat::Mode2:
      return "Mode2";
    case SPIFormat::Mode3:
      return "Mode3";
    case SPIFormat::Mode0:
    default:
      return "Mode0";
  }
}
}  // namespace

class MockSPI {
 public:
  MockSPI(nano_hw::Pin miso, nano_hw::Pin mosi, nano_hw::Pin sclk,
          int frequency, ICallbacks* callbacks, void* callback_context)
      : miso_(miso),
        mosi_(mosi),
        sclk_(sclk),
        frequency_(frequency),
        callbacks_(callbacks),
        callback_context_(callback_context) {
    std::cout << "MockSPI initialized: MOSI " << mosi_.number << ", MISO "
              << miso_.number << ", SCLK " << sclk_.number << ", frequency "
              << frequency_ << "\n";
  }

  void SetMode(SPIFormat format) {
    std::cout << "MockSPI SetMode: " << ToModeName(format) << "\n";
  }

  void SetFrequency(int frequency) {
    frequency_ = frequency;
    std::cout << "MockSPI SetFrequency: " << frequency_ << "\n";
  }

  int Transfer(std::vector<uint8_t> const& tx, std::vector<uint8_t>& rx) {
    rx = tx;
    std::cout << "MockSPI Transfer: length " << tx.size() << ", data [";
    for (size_t i = 0; i < tx.size(); ++i) {
      std::cout << std::hex << static_cast<int>(tx[i]) << std::dec;
      if (i + 1 < tx.size()) {
        std::cout << " ";
      }
    }
    std::cout << "]\n";

    if (callbacks_ != nullptr) {
      callbacks_->OnTransfer(callback_context_, tx, rx);
    }

    return static_cast<int>(tx.size());
  }

 private:
  nano_hw::Pin miso_;
  nano_hw::Pin mosi_;
  nano_hw::Pin sclk_;
  int frequency_;
  ICallbacks* callbacks_;
  void* callback_context_;
};
}  // namespace nano_stub