#pragma once
#include "NanoHW/can.hpp"

#include <iostream>

#include "NanoHW/pin.hpp"

namespace nano_stub {
using namespace nano_hw::can;

class MockCAN {
 public:
  MockCAN(nano_hw::Pin transmit_pin, nano_hw::Pin receive_pin, int frequency,
          ICallbacks* callbacks, void* callback_context)
      : transmit_pin_(transmit_pin),
        receive_pin_(receive_pin),
        frequency_(frequency),
        callbacks_(callbacks),
        callback_context_(callback_context) {
    std::cout << "MockCAN initialized: TX pin " << transmit_pin_.number
              << ", RX pin " << receive_pin_.number << ", frequency "
              << frequency << "\n";
  }

  bool SendMessage(CANMessage msg) {
    std::cout << "CAN SendMessage: ID 0x" << std::hex << msg.id << std::dec
              << ", len " << static_cast<int>(msg.len) << ", data [";
    for (int i = 0; i < msg.len; ++i) {
      std::cout << std::hex << static_cast<int>(msg.data[i]);
      if (i < msg.len - 1)
        std::cout << " ";
    }
    std::cout << std::dec << "]\n";

    // Simulate successful transmission
    if (callbacks_) {
      callbacks_->OnCANTransmit(callback_context_, msg);
    }
    return true;
  }

  int TransmitErrors() {
    std::cout << "CAN TransmitErrors called\n";
    return 0;  // Mock always returns 0
  }

  int ReceiveErrors() {
    std::cout << "CAN ReceiveErrors called\n";
    return 0;  // Mock always returns 0
  }

  void ResetPeripherals() { std::cout << "CAN ResetPeripherals called\n"; }

  void ChangeBaudrate(int frequency) {
    frequency_ = frequency;
    std::cout << "CAN ChangeBaudrate: " << frequency << "\n";
  }

  void ChangeMode(nano_hw::can::CANMode mode) {
    std::cout << "CAN ChangeMode: "
              << (mode == nano_hw::can::CANMode::kNormal ? "Normal"
                                                         : "Loopback")
              << "\n";
  }

  bool ReceiveRaw(CANMessage& msg) {
    std::cout << "CAN ReceiveRaw called\n";
    return false;  // Mock always returns false
  }

  void SetFilter(int filter_num, CANFilter filter) {
    std::cout << "CAN SetFilter: filter_num " << filter_num << ", type ";
    if (filter.filter_type == CANFilter::Type::kMask) {
      std::cout << "Mask (mask: 0x" << std::hex
                << filter.filter.mask_filter.mask << ", id: 0x"
                << filter.filter.mask_filter.id << std::dec << ")\n";
    } else {
      std::cout << "List (id: 0x" << std::hex << filter.filter.list_filter.id
                << std::dec << ")\n";
    }
  }

  void DeactivateFilter(int filter_num, CANFilter filter) {
    std::cout << "CAN DeactivateFilter: filter_num " << filter_num << "\n";
  }

 private:
  nano_hw::Pin transmit_pin_;
  nano_hw::Pin receive_pin_;
  int frequency_;
  ICallbacks* callbacks_;
  void* callback_context_;
};
}  // namespace nano_stub