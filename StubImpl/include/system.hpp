#pragma once
#include <iostream>

#include <NanoHW/system_impl.hpp>

namespace nano_stub {
class MbedSleepManager {
 public:
  static void LockSleep() { std::cout << "LockSleepImpl called" << '\n'; }
  static void UnlockSleep() { std::cout << "UnlockSleepImpl called" << '\n'; }
};
}  // namespace nano_stub