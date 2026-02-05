#pragma once

namespace {
namespace mbed {
class CAN;
}  // namespace mbed
}  // namespace

struct HAL_CAN_TypeDef {
  class mbed::CAN* can;
};