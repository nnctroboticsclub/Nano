#pragma once

#include <Nano/clock.hpp>

namespace nano_hw {

// C++'s chrono clock
struct HighResClock_Impl {
 public:
  static uint64_t Now();
};

using HighResClock = Nano::CxxClock<HighResClock_Impl>;

}  // namespace nano_hw
