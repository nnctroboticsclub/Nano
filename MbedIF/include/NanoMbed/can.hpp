#pragma once

#include <cstring>
#include <mutex>
#include <queue>

#include <NanoHW/can.hpp>

#include "../mbed.h"
#include "common.hpp"
#include "hal.hpp"
#include "pin_name.hpp"

namespace mbed {

// CANMessage compatible with mbed API
struct CANMessage {
  uint32_t id = 0;
  uint8_t data[8]{};
  uint8_t len = 0;

  CANMessage() = default;

  CANMessage(uint32_t id, uint8_t* data, uint8_t len) : id(id), len(len) {
    std::memcpy(this->data, data, len);
  }

  // Convert to nano_hw::can::CANMessage
  explicit operator nano_hw::can::CANMessage() const {
    nano_hw::can::CANMessage msg;
    msg.id = this->id;
    msg.len = this->len;
    std::memcpy(msg.data, this->data, this->len);
    return msg;
  }

  // Convert from nano_hw::can::CANMessage
  static CANMessage from_nano_hw(const nano_hw::can::CANMessage& nano_msg) {
    CANMessage msg;
    msg.id = nano_msg.id;
    msg.len = nano_msg.len;
    std::memcpy(msg.data, nano_msg.data, nano_msg.len);
    return msg;
  }
};

enum CANFormat : uint8_t { CANStandard, CANExtended, CANAny };
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

struct HAL_CAN_TypeDef {
  class CAN* can;
};

struct can_t {
  HAL_CAN_TypeDef CanHandle;
};

class CAN {
  struct MbedCANConfig {
    struct OnCANReceived {
      static void execute(void* context, nano_hw::can::CANMessage msg) {
        auto* can = static_cast<CAN*>(context);
        if (can == nullptr) {
          return;
        }
        std::lock_guard<std::mutex> lock(can->rx_mutex);
        can->rx_queue.push(msg);

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
      : CAN(receive_pin, send_pin, (size_t)1e6) {}

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
    std::lock_guard<std::mutex> lock(rx_mutex);

    if (rx_queue.empty()) {
      return 0;  // No message available
    }

    // Get message from queue
    nano_hw::can::CANMessage nano_msg = rx_queue.front();
    rx_queue.pop();

    // Convert to mbed::CANMessage
    msg = CANMessage::from_nano_hw(nano_msg);
    return 1;  // Message successfully read
  }

  void reset() { dri_.ResetPeripherals(); }

  void attach(Callback<void()> cb, IrqType irq_type) {
    switch (irq_type) {
      case RxIrq:
        rx_callback = cb;
        break;
      case TxIrq:
        tx_callback = cb;
        break;
      case BeIrq:
        bus_error_callback = cb;
        break;
      case EpIrq:
        passive_error_callback = cb;
        break;
    }
  }

 protected:
  can_t _can;

 private:
  std::queue<nano_hw::can::CANMessage> rx_queue;
  std::mutex rx_mutex;

  Callback<void()> rx_callback{};
  Callback<void()> tx_callback{};
  Callback<void()> bus_error_callback{};
  Callback<void()> passive_error_callback{};

  nano_hw::can::DynCAN<MbedCANConfig> dri_;

  friend HAL_StatusTypeDef HAL_CAN_ConfigFilter(
      HAL_CAN_TypeDef* hcan, CAN_FilterConfTypeDef* sFilterConfig);
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

}  // namespace mbed
