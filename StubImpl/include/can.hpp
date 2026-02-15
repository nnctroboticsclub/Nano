#pragma once
#include "NanoHW/can.hpp"

#include <iostream>

#include "NanoHW/pin.hpp"

namespace nano_stub {
using namespace nano_hw::can;

template <nano_hw::can::CANConfig Config>
class MockCAN {
 public:
  MockCAN(nano_hw::Pin transmit_pin, nano_hw::Pin receive_pin, int frequency)
      : MockCAN(transmit_pin, receive_pin, frequency, nullptr) {}
  MockCAN(nano_hw::Pin transmit_pin, nano_hw::Pin receive_pin, int frequency,
          void* ctx)
      : transmit_pin_(transmit_pin),
        receive_pin_(receive_pin),
        frequency_(frequency),
        context_(ctx) {
    std::cout << "MockCAN initialized: TX pin " << transmit_pin_.number
              << ", RX pin " << receive_pin_.number << ", frequency "
              << frequency << ", context " << context_ << "\n";
  }

  bool SendMessage(CANMessage msg) {
    std::cout << "CAN SendMessage: ID 0x" << std::hex << msg.id << std::dec
              << ", len " << static_cast<int>(msg.len) << ", data [";
    for (int i = 0; i < msg.len; ++i) {
      std::cout << std::hex << static_cast<int>(msg.data[i]);
      if (i < msg.len - 1)
        std::cout << " ";
    }
    std::cout << std::dec;
    std::cout << "("
              << (msg.format == nano_hw::can::CANMessageFormat::kStandard
                      ? "Standard"
                      : "Extended")
              << ")";
    std::cout << "]\n";

    // Simulate successful transmission
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

  // Simulate receiving a CAN message and invoke the callback
  void SimulateReceive(CANMessage msg) {
    std::cout << "MockCAN SimulateReceive: ID 0x" << std::hex << msg.id
              << std::dec << ", len " << static_cast<int>(msg.len) << "\n";
    Config::OnCANReceived::execute(context_, msg);
  }

  // Simulate a bus error event
  void SimulateBusError() {
    std::cout << "MockCAN SimulateBusError\n";
    Config::OnCANBusError::execute(context_);
  }

  // Simulate a passive error event
  void SimulatePassiveError() {
    std::cout << "MockCAN SimulatePassiveError\n";
    Config::OnCANPassiveError::execute(context_);
  }

  // Try to receive a CAN message (CANWithPolling support)
  bool TryReceive(CANMessage& msg) {
    std::cout << "MockCAN TryReceive called\n";
    // Mock implementation: no message available by default
    return false;
  }

 private:
  nano_hw::Pin transmit_pin_;
  nano_hw::Pin receive_pin_;
  int frequency_;
  void* context_;
};

static_assert(nano_hw::can::CAN<MockCAN>);
static_assert(nano_hw::can::CANWithPolling<MockCAN>);

}  // namespace nano_stub