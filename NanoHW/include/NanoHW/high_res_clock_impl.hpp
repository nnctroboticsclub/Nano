#pragma once

#include "high_res_clock.hpp"

namespace nano_hw {

// Friend-Injection 用の Impl 関数宣言
std::chrono::milliseconds HighResClock_NowImpl();

/// @brief HighResClockLike concept を満たす実装クラスから動的ディスパッチ関数を生成
/// @tparam Impl HighResClockLike concept を満たす実装クラス
template <HighResClockLike Impl>
class HighResClockImpl {
  friend std::chrono::milliseconds HighResClock_NowImpl() {
    return Impl::Now();
  }
};

// Friend-Injection で定義された関数はオブジェクトファイルに含まれないため、
// 通常の関数を挟んで実体を残す
std::chrono::milliseconds HighResClock_Now() {
  return HighResClock_NowImpl();
}

}  // namespace nano_hw
