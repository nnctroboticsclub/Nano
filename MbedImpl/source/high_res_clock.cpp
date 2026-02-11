#include <mbed.h>

#include <NanoHW/high_res_clock_impl.hpp>

class MbedHighResClock {
 public:
  static std::chrono::milliseconds Now() {
    auto d = mbed::HighResClock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(d);
  }
};

template struct nano_hw::HighResClockImpl<MbedHighResClock>;
