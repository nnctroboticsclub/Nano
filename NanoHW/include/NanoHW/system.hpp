#pragma once

namespace nano_hw {

template <typename T>
concept SleepManager = requires() {
  T::LockSleep();
  T::UnlockSleep();
};

void LockSleep();
void UnlockSleep();

class DynSleepManager {
 public:
  static void LockSleep() { nano_hw::LockSleep(); }
  static void UnlockSleep() { nano_hw::UnlockSleep(); }
};

static_assert(SleepManager<DynSleepManager>);

}  // namespace nano_hw