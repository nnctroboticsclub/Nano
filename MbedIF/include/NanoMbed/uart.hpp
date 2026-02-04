#pragma once

#include <cstddef>
#include <cstdint>

#include <NanoHW/uart.hpp>

#include "pin_name.hpp"

namespace mbed {

class UnbufferedSerial {
 public:
  UnbufferedSerial(PinName tx, PinName rx, int baud_rate = kDefaultBaudrate)
      : dri_(ToPin(tx), ToPin(rx), baud_rate) {}

  void baud(int baud_rate) { dri_.Rebaud(baud_rate); }

  size_t write(const void* buffer, size_t size) {
    return dri_.Send(const_cast<void*>(buffer), size);
  }

  size_t read(void* buffer, size_t size) { return dri_.Receive(buffer, size); }

 private:
  static constexpr int kDefaultBaudrate = 115200;

  nano_hw::uart::DynUART<nano_hw::uart::DummyUARTConfig> dri_;
};

}  // namespace mbed
