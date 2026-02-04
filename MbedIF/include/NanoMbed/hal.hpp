#pragma once

#include <cstdint>

enum HAL_StatusTypeDef : uint8_t {
  HAL_OK = 0,
};

enum EnableState : uint8_t {
  DISABLE,
  ENABLE,
};
