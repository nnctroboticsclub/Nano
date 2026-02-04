#pragma once

#include <chrono>
#include <cstdio>
#include <cstring>

#include <NanoMbed/can.hpp>
#include <NanoMbed/digital_out.hpp>
#include "NanoHW/parallel.hpp"

namespace mbed {

// Timer
class Timer {
 public:
  void reset();
  void start();

  int read_ms();
};

// RTOS
namespace ThisThread {
void sleep_for(std::chrono::milliseconds ms) {
  nano_hw::parallel::SleepForMS(ms);
}
}  // namespace ThisThread
}  // namespace mbed

using namespace mbed;
using namespace std;