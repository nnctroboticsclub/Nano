#pragma once

#include <chrono>
#include <cstdio>
#include <cstring>

#include <NanoHW/parallel.hpp>
#include <NanoMbed/can.hpp>
#include <NanoMbed/digital_out.hpp>
#include <NanoMbed/timer.hpp>

namespace mbed {

// RTOS
namespace ThisThread {
inline void sleep_for(std::chrono::milliseconds ms) {
  nano_hw::parallel::SleepForMS(ms);
}
}  // namespace ThisThread
}  // namespace mbed

using namespace mbed;
using namespace std;