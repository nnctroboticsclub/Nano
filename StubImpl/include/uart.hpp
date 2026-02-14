#pragma once

#include <NanoHW/pin.hpp>
#include <NanoHW/uart.hpp>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace nano_stub {

template <nano_hw::uart::UARTConfig Config>
class MockUART {
 public:
  MockUART(nano_hw::Pin tx, nano_hw::Pin rx, int baud_rate)
      : tx_(tx), rx_(rx), baud_rate_(baud_rate) {
    std::cout << "MockUART initialized: TX " << tx_.number << ", RX "
              << rx_.number << ", baud " << baud_rate_ << "\n";
  }

  void Rebaud(int baud_rate) {
    baud_rate_ = baud_rate;
    std::cout << "MockUART Rebaud: " << baud_rate_ << "\n";
  }

  size_t Send(void* buffer, size_t size) {
    std::cout << "MockUART Send: size " << size << "\n";
    return size;
  }

  size_t Receive(void* buffer, size_t size) {
    std::cout << "MockUART Receive: size " << size << "\n";
    if (buffer != nullptr && size > 0) {
      std::memset(buffer, 0xA5, size);
    }
    return size;
  }

  // Simulate receiving data and invoke the callback
  void SimulateReceive(const uint8_t* data, size_t size) {
    std::cout << "MockUART SimulateReceive: size " << size << "\n";
    Config::OnUARTRx::execute(nullptr, data, size);
  }

  // Simulate transmission complete and invoke the callback
  void SimulateTransmitComplete(size_t size) {
    std::cout << "MockUART SimulateTransmitComplete: size " << size << "\n";
    Config::OnUARTTx::execute(nullptr, nullptr, size);
  }

 private:
  nano_hw::Pin tx_;
  nano_hw::Pin rx_;
  int baud_rate_;
};

static_assert(nano_hw::uart::UART<MockUART>);

}  // namespace nano_stub