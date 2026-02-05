#pragma once

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

#include <NanoHW/parallel.hpp>
#include <NanoMbed/can.hpp>
#include <NanoMbed/digital_out.hpp>
#include <NanoMbed/spi.hpp>
#include <NanoMbed/thread.hpp>
#include <NanoMbed/timer.hpp>
#include <NanoMbed/uart.hpp>

namespace {

namespace mbed::ThisThread {
inline void sleep_for(std::chrono::milliseconds ms) {
  nano_hw::parallel::SleepForMS(ms);
}
}  // namespace mbed::ThisThread

}  // namespace

using namespace mbed;
using namespace rtos;
using namespace std;