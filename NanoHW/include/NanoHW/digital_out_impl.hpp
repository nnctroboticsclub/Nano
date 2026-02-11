#pragma once

#include "digital_out.hpp"

namespace nano_hw {

// Friend-Injection 用の Impl 関数宣言
void* AllocDigitalOutInstanceImpl(Pin pin);
void FreeDigitalOutInstanceImpl(void* inst);
void WriteDigitalOutImpl(void* inst, bool state);
bool ReadDigitalOutImpl(void* inst);

/// @brief DigitalOut concept を満たす実装クラスから動的ディスパッチ関数を生成
/// @tparam Impl DigitalOut concept を満たす実装クラス
template <DigitalOut Impl>
class DigitalOutImpl {
  friend void* AllocDigitalOutInstanceImpl(Pin pin) { return new Impl(pin); }

  friend void FreeDigitalOutInstanceImpl(void* inst) {
    delete static_cast<Impl*>(inst);
  }

  friend void WriteDigitalOutImpl(void* inst, bool state) {
    static_cast<Impl*>(inst)->Write(state);
  }

  friend bool ReadDigitalOutImpl(void* inst) {
    return static_cast<Impl*>(inst)->Read();
  }
};

// Friend-Injection で定義された関数はオブジェクトファイルに含まれないため、
// 通常の関数を挟んで実体を残す
void* AllocDigitalOutInstance(Pin pin) {
  return AllocDigitalOutInstanceImpl(pin);
}
void FreeDigitalOutInstance(void* inst) {
  FreeDigitalOutInstanceImpl(inst);
}
void WriteDigitalOut(void* inst, bool state) {
  WriteDigitalOutImpl(inst, state);
}
bool ReadDigitalOut(void* inst) {
  return ReadDigitalOutImpl(inst);
}

}  // namespace nano_hw
