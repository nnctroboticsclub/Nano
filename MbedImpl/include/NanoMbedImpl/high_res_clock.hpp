#pragma once

#include <mbed.h>

#include <NanoHW/high_res_clock_impl.hpp>

namespace nano_mbed {
class MbedHighResClock {
 public:
  static std::chrono::milliseconds Now() {
    auto d = mbed::HighResClock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(d);
  }
};
}  // namespace nano_mbed