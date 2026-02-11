#pragma once

#include "pwm.hpp"

namespace nano_hw {

// Friend-Injection 用の Impl 関数宣言
void* AllocPwmOutInstanceImpl(Pin pin);
void FreePwmOutInstanceImpl(void* inst);
void WritePwmOutImpl(void* inst, float duty_cycle);
float ReadPwmOutImpl(void* inst);
void SetPeriodPwmOutImpl(void* inst, float period_s);

/// @brief PwmOut concept を満たす実装クラスから動的ディスパッチ関数を生成
/// @tparam Impl PwmOut concept を満たす実装クラス
template <PwmOut Impl>
class PwmOutImpl {
  friend void* AllocPwmOutInstanceImpl(Pin pin) { return new Impl(pin); }

  friend void FreePwmOutInstanceImpl(void* inst) {
    delete static_cast<Impl*>(inst);
  }

  friend void WritePwmOutImpl(void* inst, float duty_cycle) {
    static_cast<Impl*>(inst)->Write(duty_cycle);
  }

  friend float ReadPwmOutImpl(void* inst) {
    return static_cast<Impl*>(inst)->Read();
  }

  friend void SetPeriodPwmOutImpl(void* inst, float period_s) {
    static_cast<Impl*>(inst)->SetPeriod(period_s);
  }
};

// Friend-Injection で定義された関数はオブジェクトファイルに含まれないため、
// 通常の関数を挟んで実体を残す
void* AllocPwmOutInstance(Pin pin) {
  return AllocPwmOutInstanceImpl(pin);
}
void FreePwmOutInstance(void* inst) {
  FreePwmOutInstanceImpl(inst);
}
void WritePwmOut(void* inst, float duty_cycle) {
  WritePwmOutImpl(inst, duty_cycle);
}
float ReadPwmOut(void* inst) {
  return ReadPwmOutImpl(inst);
}
void SetPeriodPwmOut(void* inst, float period_s) {
  SetPeriodPwmOutImpl(inst, period_s);
}

}  // namespace nano_hw
