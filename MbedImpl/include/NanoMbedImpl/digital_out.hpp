#pragma once
#include <mbed.h>
#include <NanoHW/digital_out.hpp>

namespace nano_mbed {
class MbedDigitalOut {
 public:
  explicit MbedDigitalOut(nano_hw::Pin pin)
      : digital_out_(static_cast<PinName>(pin.number)) {}

  void Write(bool state) { digital_out_.write(state ? 1 : 0); }

  bool Read() const {
    // Cast away const to call non-const read() method
    return const_cast<mbed::DigitalOut&>(digital_out_).read() != 0;
  }

 private:
  mbed::DigitalOut digital_out_;
};
}  // namespace nano_mbed
