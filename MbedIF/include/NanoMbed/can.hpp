#pragma once

#include <cstring>

#include <Nano/no_mutex_lifo.hpp>
#include <NanoHW/can.hpp>

#include "can_api.h"
#include "common.hpp"
#include "hal.hpp"
#include "pin_name.hpp"

namespace {
namespace mbed {

enum CANFilterMode_t : uint8_t {
  CAN_FILTERMODE_IDMASK = 0,
  CAN_FILTERMODE_IDLIST = 1,
};
enum CANFilterScale_t : uint8_t {
  CAN_FILTERSCALE_16BIT = 0,
  CAN_FILTERSCALE_32BIT = 1,
};
enum CANFilterFIFO_t : uint8_t {
  CAN_FILTER_FIFO0 = 0,
  CAN_FILTER_FIFO1 = 1,
};
struct CAN_FilterConfTypeDef {
  int FilterNumber;
  CANFilterMode_t FilterMode;
  CANFilterScale_t FilterScale;
  uint32_t FilterIdHigh;
  uint32_t FilterIdLow;
  uint32_t FilterMaskIdHigh;
  uint32_t FilterMaskIdLow;

  CANFilterFIFO_t FilterFIFOAssignment;
  EnableState FilterActivation;
  uint8_t BankNumber;
};

class CAN {
  struct MbedCANConfig {
    struct OnCANReceived {
      static void execute(void* context, nano_hw::can::CANMessage msg) {
        auto* can = static_cast<CAN*>(context);
        if (can == nullptr) {
          return;
        }
        can->rx_queue.Push(msg);

        if (can->rx_callback) {
          can->rx_callback();
        }
      }
    };

    struct OnCANTransmit {
      static void execute(void* context, nano_hw::can::CANMessage msg) {
        (void)msg;

        auto* can = static_cast<CAN*>(context);
        if (can == nullptr) {
          return;
        }
        if (can->tx_callback) {
          can->tx_callback();
        }
      }
    };

    struct OnCANBusError {
      static void execute(void* context) {
        auto* can = static_cast<CAN*>(context);
        if (can == nullptr) {
          return;
        }
        if (can->bus_error_callback) {
          can->bus_error_callback();
        }
      }
    };

    struct OnCANPassiveError {
      static void execute(void* context) {
        auto* can = static_cast<CAN*>(context);
        if (can == nullptr) {
          return;
        }
        if (can->passive_error_callback) {
          can->passive_error_callback();
        }
      }
    };
  };

 public:
  enum IrqType : uint8_t {
    RxIrq,
    TxIrq,
    BeIrq,
    EpIrq,
  };

  CAN(PinName receive_pin, PinName send_pin)
      : CAN(receive_pin, send_pin, static_cast<size_t>(1e6)) {}

  CAN(PinName receive_pin, PinName send_pin, int frequency)
      : dri_(ToPin(send_pin), ToPin(receive_pin), frequency, this), _can() {
    _can.CanHandle.can = this;
  }

  virtual ~CAN() = default;

  // Lock handling to be implemented if needed
  virtual void lock() {}
  virtual void unlock() {}

  int rderror() { return dri_.ReceiveErrors(); }
  int tderror() { return dri_.TransmitErrors(); }

  int write(const CANMessage& msg) {
    // Convert mbed::CANMessage to nano_hw::can::CANMessage
    auto nano_msg = static_cast<nano_hw::can::CANMessage>(msg);
    return dri_.SendMessage(nano_msg) ? 1 : 0;
  }

  int read(CANMessage& msg, int handle = 0) {
    (void)handle;
    if (rx_queue.Empty()) {
      return 0;  // No message available
    }

    // Get message from queue
    nano_hw::can::CANMessage nano_msg = rx_queue.Pop();

    // Convert to mbed::CANMessage
    msg = CANMessage::from_nano_hw(nano_msg);
    return 1;  // Message successfully read
  }

  void reset() { dri_.ResetPeripherals(); }

  void attach(Callback<void()> const& handler, IrqType irq_type) {
    switch (irq_type) {
      case RxIrq:
        rx_callback = handler;
        break;
      case TxIrq:
        tx_callback = handler;
        break;
      case BeIrq:
        bus_error_callback = handler;
        break;
      case EpIrq:
        passive_error_callback = handler;
        break;
    }
  }

 protected:
  can_t _can;  // NOLINT

 private:
  constexpr static size_t kRxQueueSize = 32;
  Nano::collection::NoMutexLIFO<nano_hw::can::CANMessage, kRxQueueSize>
      rx_queue;

  Callback<void()> rx_callback;
  Callback<void()> tx_callback;
  Callback<void()> bus_error_callback;
  Callback<void()> passive_error_callback;

  nano_hw::can::DynCAN<MbedCANConfig> dri_;

  friend HAL_StatusTypeDef HAL_CAN_ConfigFilter(
      HAL_CAN_TypeDef* hcan, CAN_FilterConfTypeDef* sFilterConfig);
  friend int can_read(can_t* obj, CANMessage* msg, int handle);
};

inline HAL_StatusTypeDef HAL_CAN_ConfigFilter(
    HAL_CAN_TypeDef* hcan, CAN_FilterConfTypeDef* sFilterConfig) {
  nano_hw::can::CANFilter filter;
  const uint32_t id =
      sFilterConfig->FilterIdHigh << 16 | sFilterConfig->FilterIdLow;
  if (sFilterConfig->FilterMode == CAN_FILTERMODE_IDMASK) {
    const uint32_t mask =
        sFilterConfig->FilterMaskIdHigh << 16 | sFilterConfig->FilterMaskIdLow;

    filter.filter_type = nano_hw::can::CANFilter::Type::kMask;
    filter.filter.mask_filter.id = id;
    filter.filter.mask_filter.mask = mask;
  } else {
    filter.filter_type = nano_hw::can::CANFilter::Type::kList;
    filter.filter.list_filter.id = id;
  }

  hcan->can->dri_.SetFilter(sFilterConfig->FilterNumber, filter);
  return HAL_OK;
}

inline int can_read(can_t* obj, CANMessage* msg, int handle) {
  (void)handle;
  nano_hw::can::CANMessage nano_msg;
  bool result = nano_hw::can::ReceiveRawImpl(obj->CanHandle.can, nano_msg);
  if (!result) {
    return 0;  // No message arrived
  }

  // Convert to CANMessage
  msg->id = nano_msg.id;
  msg->len = nano_msg.len;
  std::memcpy(msg->data, nano_msg.data, nano_msg.len);
  return 1;  // Message arrived
}

}  // namespace mbed

}  // namespace