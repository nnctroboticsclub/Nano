#pragma once

#include "system.hpp"

namespace nano_hw {

// Friend-Injection 用の Impl 関数宣言
void LockSleepImpl();
void UnlockSleepImpl();

/// @brief SleepManager concept を満たす実装クラスから動的ディスパッチ関数を生成
/// @tparam Impl SleepManager concept を満たす実装クラス
template <SleepManager Impl>
class SleepManagerImpl {
  friend void LockSleepImpl() { Impl::LockSleep(); }

  friend void UnlockSleepImpl() { Impl::UnlockSleep(); }
};

// Friend-Injection で定義された関数はオブジェクトファイルに含まれないため、
// 通常の関数を挟んで実体を残す
void LockSleep() {
  return LockSleepImpl();
}
void UnlockSleep() {
  UnlockSleepImpl();
}

}  // namespace nano_hw
