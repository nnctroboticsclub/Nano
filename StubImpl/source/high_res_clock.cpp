#include <NanoHW/high_res_clock.hpp>
#include <chrono>

uint64_t nano_hw::HighResClock_Impl::Now() {
  auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::microseconds>(time).count();
}
