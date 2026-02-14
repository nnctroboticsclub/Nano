#pragma once
#include <NanoHW/pin.hpp>
#include <NanoHW/spi.hpp>

#include <iostream>
#include <vector>

namespace nano_stub {
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

template <nano_hw::spi::SPIConfig Config>
class MockSPI {
 public:
  MockSPI(nano_hw::Pin miso, nano_hw::Pin mosi, nano_hw::Pin sclk,
          int frequency)
      : miso_(miso), mosi_(mosi), sclk_(sclk), frequency_(frequency) {
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

    return static_cast<int>(tx.size());
  }

  // Simulate transfer complete and invoke the callback
  void SimulateTransferComplete(std::vector<uint8_t> const& rx_data) {
    std::cout << "MockSPI SimulateTransferComplete: size " << rx_data.size()
              << "\n";
    Config::OnSPITransferComplete::execute(nullptr, rx_data);
  }

 private:
  nano_hw::Pin miso_;
  nano_hw::Pin mosi_;
  nano_hw::Pin sclk_;
  int frequency_;
};

static_assert(nano_hw::spi::SPI<MockSPI>);

}  // namespace nano_stub