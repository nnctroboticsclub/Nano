#pragma once

#include <chrono>

#include <Nano/clock.hpp>

#include "NanoHW/high_res_clock.hpp"
#include "NanoHW/system.hpp"

namespace {
namespace mbed {

class HighResClock : public nano_hw::HighResClock {
  static void lock() { nano_hw::DynSleepManager::LockSleep(); }
  static void unlock() { nano_hw::DynSleepManager::UnlockSleep(); }
};
static_assert(std::chrono::is_clock_v<HighResClock>);

}  // namespace mbed
}  // namespace