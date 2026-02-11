#pragma once

#include <NanoHW/high_res_clock.hpp>
#include <chrono>

namespace nano_stub {
class StubHighResClock {
 public:
  static std::chrono::milliseconds Now() {
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(time);
  }
};
}  // namespace nano_stub