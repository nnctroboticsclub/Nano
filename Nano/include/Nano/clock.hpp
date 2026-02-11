#pragma once

#include <chrono>
#include <concepts>

namespace Nano {

template <typename T, typename Dur>
concept Clock = requires() {
  {T::Now()}->std::same_as<Dur>;
};

struct DummyClock {
  static std::chrono::microseconds Now();
};

template <typename Duration, Clock<Duration> Clk>
class CxxClock {
 public:
  using duration = Duration;
  using rep = duration::rep;
  using period = duration::period;
  using time_point = std::chrono::time_point<CxxClock>;
  static const bool is_steady = false;

  static time_point now() { return time_point(duration(Clk::Now())); }
};
static_assert(
    std::chrono::is_clock_v<CxxClock<std::chrono::microseconds, DummyClock>>);

}  // namespace Nano