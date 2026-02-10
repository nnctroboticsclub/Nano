#include <mbed.h>
#include <NanoHW/high_res_clock.hpp>

uint64_t nano_hw::HighResClock_Impl::Now() {
  return mbed::HighResClock::now().time_since_epoch().count();
}
