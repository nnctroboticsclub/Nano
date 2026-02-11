#pragma once

#include <Nano/clock.hpp>
#include <chrono>
#include <concepts>

namespace nano_hw {

using HighResClockDuration = std::chrono::milliseconds;

template <typename T>
concept HighResClockLike = requires() {
  {T::Now()}->std::same_as<HighResClockDuration>;
};

template <HighResClockLike T>
using HighResClock = Nano::CxxClock<HighResClockDuration, T>;

HighResClockDuration HighResClock_Now();

class DynHighResClock {
 public:
  static auto Now() { return nano_hw::HighResClock_Now(); }
};

static_assert(HighResClockLike<DynHighResClock>);

}  // namespace nano_hw
