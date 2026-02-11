#pragma once
#include <NanoHW/system.hpp>

#include "platform/mbed_power_mgmt.h"

namespace nano_mbed {
class MbedSleepManager {
 public:
  static void LockSleep() { sleep_manager_lock_deep_sleep(); }
  static void UnlockSleep() { sleep_manager_unlock_deep_sleep(); }
};
}  // namespace nano_mbed