#pragma once
#include "NanoHW/digital_out_impl.hpp"
#include "NanoHW/pin.hpp"

#include <iostream>

namespace nano_stub {
class MockDigitalOut {
 public:
  explicit MockDigitalOut(nano_hw::Pin pin) : pin_(pin) {}

  void Write(bool state) const {
    std::cout << "DigitalOut on pin " << pin_.number << " set to " << state
              << "\n";
  }

  bool Read() const {
    std::cout << "DigitalOut on pin " << pin_.number << " read\n";
    return false;  // Mock always returns false
  }

 private:
  nano_hw::Pin pin_;
};
}  // namespace nano_stub