#include <iostream>

#include <NanoHW/system_impl.hpp>

class MbedSleepManager {
 public:
  static void LockSleep() { std::cout << "LockSleepImpl called" << '\n'; }
  static void UnlockSleep() { std::cout << "UnlockSleepImpl called" << '\n'; }
};

template class nano_hw::SleepManagerImpl<MbedSleepManager>;