#pragma once

#include <NanoHW/digital_out.hpp>

#include "pin_name.hpp"

namespace mbed {
class DigitalOut {
 public:
  explicit(false) DigitalOut(PinName pin) : dri_(ToPin(pin)) {}
  void write(bool state) { dri_.Write(state); }
  bool read() { return dri_.Read(); }

  operator bool() { return read(); }
  DigitalOut& operator=(bool value) {
    write(value);
    return *this;
  }

 private:
  nano_hw::DynDigitalOut dri_;
};

}  // namespace mbed