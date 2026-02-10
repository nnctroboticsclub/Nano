#include <NanoHW/system.hpp>

#include "platform/mbed_power_mgmt.h"

void nano_hw::LockSleepImpl() {
  sleep_manager_lock_deep_sleep();
}
void nano_hw::UnlockSleepImpl() {
  sleep_manager_unlock_deep_sleep();
}