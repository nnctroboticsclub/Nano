#pragma once

#include <mbed.h>
#include <NanoHW/can_impl.hpp>

namespace nano_mbed {
using nano_hw::can::CANFilter;
using nano_hw::can::ICallbacks;
using HWCANMessage = nano_hw::can::CANMessage;
using MbedCANMessage = mbed::CANMessage;

namespace {
class CANHack_t : private mbed::CAN {
 public:
  static std::size_t kOffset_can_t;
};
size_t CANHack_t::kOffset_can_t = offsetof(CANHack_t, _can);

static_assert(sizeof(CANHack_t) == sizeof(mbed::CAN));

static auto GetCANAPI(mbed::CAN& can) -> can_t* {
  auto ptr_can = reinterpret_cast<uintptr_t>(&can) + CANHack_t::kOffset_can_t;
  return reinterpret_cast<can_t*>(ptr_can);
}
}  // namespace

class MbedCAN {
 public:
  MbedCAN(nano_hw::Pin transmit_pin, nano_hw::Pin receive_pin, int frequency,
          ICallbacks* callbacks, void* callback_context)
      : callbacks_(callbacks),
        callback_context_(callback_context),
        can_(static_cast<PinName>(receive_pin.number),
             static_cast<PinName>(transmit_pin.number)) {
    can_.frequency(frequency);

    // Attach RX interrupt handler
    can_.attach(mbed::callback(this, &MbedCAN::OnReceive), mbed::CAN::RxIrq);
  }

  ~MbedCAN() { can_.attach(nullptr, mbed::CAN::RxIrq); }

  bool SendMessage(HWCANMessage msg) {
    MbedCANMessage mbed_msg(msg.id);
    for (int i = 0; i < msg.len && i < 8; ++i) {
      mbed_msg.data[i] = msg.data[i];
    }
    mbed_msg.len = msg.len;
    int result = can_.write(mbed_msg);

    if (result == 1 && callbacks_ != nullptr) {
      callbacks_->OnCANTransmit(callback_context_, msg);
    }

    return result == 1;
  }

  int TransmitErrors() { return can_.tderror(); }

  int ReceiveErrors() { return can_.rderror(); }

  void ResetPeripherals() { can_.reset(); }

  void ChangeBaudrate(int frequency) { can_.frequency(frequency); }

  void ChangeMode(nano_hw::can::CANMode mode) {
    // Mbed doesn't support loopback mode directly
    (void)mode;
  }

  void SetFilter(int filter_num, CANFilter filter) {
    if (filter.filter_type == CANFilter::Type::kMask) {
      can_.filter(filter.filter.mask_filter.id, filter.filter.mask_filter.mask,
                  CANStandard, filter_num);
    } else {
      can_.filter(filter.filter.list_filter.id, 0xFFFFFFFF, CANStandard,
                  filter_num);
    }
  }

  void DeactivateFilter(int filter_num, CANFilter filter) {
    // Mbed doesn't have explicit deactivate, set to accept nothing
    can_.filter(0, 0, CANStandard, filter_num);
  }

  bool ReceiveRaw(HWCANMessage& msg) {
    auto* hal_can = GetCANAPI(can_);

    MbedCANMessage mbed_msg;
    if (can_read(hal_can, reinterpret_cast<CAN_Message*>(&mbed_msg), 0)) {
      msg.id = mbed_msg.id;
      msg.len = mbed_msg.len;
      for (int i = 0; i < mbed_msg.len && i < 8; ++i) {
        msg.data[i] = mbed_msg.data[i];
      }
      return true;
    }
    return false;
  }

 private:
  void OnReceive() {
    MbedCANMessage mbed_msg;
    if (can_.read(mbed_msg)) {
      HWCANMessage msg;
      msg.id = mbed_msg.id;
      msg.len = mbed_msg.len;
      for (int i = 0; i < mbed_msg.len && i < 8; ++i) {
        msg.data[i] = mbed_msg.data[i];
      }

      if (callbacks_ != nullptr) {
        callbacks_->OnCANReceived(callback_context_, msg);
      }
    }
  }

  ICallbacks* callbacks_;
  void* callback_context_;

 public:
  mbed::CAN can_;
};
}  // namespace nano_mbed