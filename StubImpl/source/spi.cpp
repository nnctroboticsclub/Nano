#include <NanoHW/pin.hpp>
#include <NanoHW/spi.hpp>

#include <iostream>
#include <vector>

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

void* nano_hw::spi::AllocInterface(nano_hw::Pin miso, nano_hw::Pin mosi,
                                   nano_hw::Pin sclk, int frequency,
                                   ICallbacks* callbacks,
                                   void* callback_context) {
  return new MockSPI(miso, mosi, sclk, frequency, callbacks, callback_context);
}

void nano_hw::spi::FreeInterface(void* interface) {
  delete static_cast<MockSPI*>(interface);
}

void nano_hw::spi::SetModeImpl(void* interface, SPIFormat format) {
  static_cast<MockSPI*>(interface)->SetMode(format);
}

void nano_hw::spi::SetFrequencyImpl(void* interface, int frequency) {
  static_cast<MockSPI*>(interface)->SetFrequency(frequency);
}

int nano_hw::spi::TransferImpl(void* interface,
                               std::vector<uint8_t> const& tx_buffer,
                               std::vector<uint8_t>& rx_buffer) {
  return static_cast<MockSPI*>(interface)->Transfer(tx_buffer, rx_buffer);
}
