#pragma once
#include <mbed.h>
#include <NanoHW/parallel.hpp>

namespace nano_hw::parallel {
void SleepForMS(std::chrono::milliseconds milliseconds) {
  ThisThread::sleep_for(milliseconds);
}
}  // namespace nano_hw::parallel