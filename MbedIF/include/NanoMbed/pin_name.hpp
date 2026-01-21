#pragma once

#include <cstdint>

#include <NanoHW/pin.hpp>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define PAD_NUM_A 0
#define PAD_NUM_B 16
#define PAD_NUM_C 32
#define PAD_NUM_D 48
#define PAD_NUM_E 64
#define PAD_NUM_F 80
#define PAD_NUM_G 96
#define PAD_NUM_H 112
#define PAD_NUM(PAD) (PAD_NUM_##PAD)
// NOLINTNEXTLINE (bugprone-macro-parentheses)
#define PIN(PAD, NUM) P##PAD##_##NUM = PAD_NUM(PAD) + NUM
// clang-format off
#define PINS(PAD) \
  PIN(PAD,  0), PIN(PAD,  1), PIN(PAD,  2), PIN(PAD,  3), \
  PIN(PAD,  4), PIN(PAD,  5), PIN(PAD,  6), PIN(PAD,  7), \
  PIN(PAD,  8), PIN(PAD,  9), PIN(PAD, 10), PIN(PAD, 11), \
  PIN(PAD, 12), PIN(PAD, 13), PIN(PAD, 14), PIN(PAD, 15)
// clang-format on
// NOLINTEND(cppcoreguidelines-macro-usage)

namespace mbed {
enum PinName : uint16_t {
  PINS(A),
  PINS(B),
  PINS(C),
  PINS(D),
  PINS(E),
  PINS(F),
  PINS(G),
  PINS(H),
  NC = 0xFFFF
};

inline nano_hw::Pin ToPin(PinName pin) {
  return nano_hw::Pin{static_cast<uint16_t>(pin)};
}

inline PinName FromPin(const nano_hw::Pin& pin) {
  return static_cast<PinName>(pin.number);
}
}  // namespace mbed

#undef PAD_NUM_A
#undef PAD_NUM_B
#undef PAD_NUM_C
#undef PAD_NUM_D
#undef PAD_NUM_E
#undef PAD_NUM_F
#undef PAD_NUM_G
#undef PAD_NUM_H
#undef PAD_NUM
#undef PIN
#undef PINS