#pragma once

namespace nano_hw {

template <typename T>
concept SleepManager = requires() {
  T::LockSleep();
  T::UnlockSleep();
};

void LockSleepImpl();
void UnlockSleepImpl();

class DynSleepManager {
 public:
  static void LockSleep() { LockSleepImpl(); }
  static void UnlockSleep() { UnlockSleepImpl(); }
};

static_assert(SleepManager<DynSleepManager>);

}  // namespace nano_hw