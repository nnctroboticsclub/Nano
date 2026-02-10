#pragma once

#include <chrono>
#include <concepts>

namespace Nano {

template <typename T>
concept Clock = requires() {
  // Now() returns current time in microseconds
  {T::Now()}->std::same_as<uint64_t>;
};

struct DummyClock {
  static uint64_t Now();
};

template <Clock Clk>
class CxxClock {
 public:
  using duration = std::chrono::microseconds;
  using rep = duration::rep;
  using period = duration::period;
  using time_point = std::chrono::time_point<CxxClock>;
  static const bool is_steady = false;

  static time_point now() { return time_point(duration(Clk::Now())); }
};
static_assert(std::chrono::is_clock_v<CxxClock<DummyClock>>);

}  // namespace Nano