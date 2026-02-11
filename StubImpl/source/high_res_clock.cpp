#include <NanoHW/high_res_clock.hpp>
#include <chrono>
#include "NanoHW/high_res_clock_impl.hpp"

class StubHighResClock {
 public:
  static std::chrono::milliseconds Now() {
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(time);
  }
};

template struct nano_hw::HighResClockImpl<StubHighResClock>;
