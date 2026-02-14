#pragma once
#include <mbed.h>
#include <NanoHW/spi.hpp>

#include <vector>

namespace nano_mbed {
using nano_hw::spi::SPIFormat;

namespace {
int ToMbedMode(SPIFormat format) {
  switch (format) {
    case SPIFormat::Mode1:
      return 1;
    case SPIFormat::Mode2:
      return 2;
    case SPIFormat::Mode3:
      return 3;
    case SPIFormat::Mode0:
    default:
      return 0;
  }
}
}  // namespace

template <nano_hw::spi::SPIConfig Config>
class MbedSPI {
 public:
  MbedSPI(nano_hw::Pin miso, nano_hw::Pin mosi, nano_hw::Pin sclk,
          int frequency)
      : spi_(static_cast<PinName>(mosi.number),
             static_cast<PinName>(miso.number),
             static_cast<PinName>(sclk.number)) {
    spi_.frequency(frequency);
  }

  void SetMode(SPIFormat format) { spi_.format(8, ToMbedMode(format)); }

  void SetFrequency(int frequency) { spi_.frequency(frequency); }

  int Transfer(std::vector<uint8_t> const& tx, std::vector<uint8_t>& rx) {
    rx.resize(tx.size());
    for (size_t i = 0; i < tx.size(); ++i) {
      rx[i] = static_cast<uint8_t>(spi_.write(tx[i]));
    }

    Config::OnTransfer::execute(this, tx, rx);

    return static_cast<int>(tx.size());
  }

 private:
  mbed::SPI spi_;
};

static_assert(nano_hw::spi::SPI<MbedSPI>);

}  // namespace nano_mbed