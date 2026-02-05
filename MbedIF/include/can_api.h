#pragma once

#include <cstdint>
#include <cstring>

#include <NanoHAL/can.h>
#include <NanoHW/can.hpp>

enum CANFormat : uint8_t { CANStandard = 0, CANExtended = 1, CANAny = 2 };

enum CANType : uint8_t { CANData = 0, CANRemote = 1 };

struct CANMessage {
  // NOLINTBEGIN
  unsigned int id = 0;             ///< 29 bit identifier
  unsigned char data[8] = {};      ///< Data field
  unsigned char len = 0;           ///< Length of data field in bytes
  CANFormat format = CANStandard;  ///< Format ::CANFormat
  CANType type = CANData;          ///< Type ::CANType
  // NOLINTEND

  CANMessage() = default;

  CANMessage(uint32_t msg_id, uint8_t* data, uint8_t len)
      : id(msg_id), len(len) {
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

using can_t = struct can_s {
  HAL_CAN_TypeDef CanHandle;
};
