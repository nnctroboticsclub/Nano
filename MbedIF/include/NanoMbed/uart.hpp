#pragma once

#include <cstddef>
#include <cstdint>

#include <NanoHW/uart.hpp>

#include "common.hpp"
#include "pin_name.hpp"

namespace {
namespace mbed {

class UnbufferedSerial {
 public:
  UnbufferedSerial(PinName tx, PinName rx, int baud_rate = kDefaultBaudrate)
      : dri_(ToPin(tx), ToPin(rx), baud_rate) {}

  void baud(int baud_rate) { dri_.Rebaud(baud_rate); }

  void close() { is_open_ = false; }

  void enable_input(bool enable) { enabled_input_ = enable; }

  void enable_output(bool enable) { enabled_output_ = enable; }

  void attach(Callback<void()> cb) { rx_callback_ = cb; }

  bool readable() const { return is_open_ && enabled_input_; }

  size_t write(const void* buffer, size_t size) {
    if (!is_open_ || !enabled_output_) {
      return 0;
    }
    return dri_.Send(const_cast<void*>(buffer), size);
  }

  size_t read(void* buffer, size_t size) {
    if (!is_open_ || !enabled_input_) {
      return 0;
    }
    auto bytes_read = dri_.Receive(buffer, size);
    if (bytes_read > 0 && rx_callback_) {
      rx_callback_();
    }
    return bytes_read;
  }

 private:
  static constexpr int kDefaultBaudrate = 115200;

  nano_hw::uart::DynUART<nano_hw::uart::DummyUARTConfig> dri_;
  Callback<void()> rx_callback_;
  bool enabled_input_ = true;
  bool enabled_output_ = true;
  bool is_open_ = true;
};

}  // namespace mbed
}  // namespace