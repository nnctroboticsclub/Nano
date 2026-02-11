#pragma once

#include <concepts>

#include <NanoHW/pin.hpp>

namespace nano_hw {
template <typename T>
concept DigitalOut = requires(T value, Pin pin, bool state) {
  {T(pin)}->std::same_as<T>;
  {value.Write(state)}->std::same_as<void>;
  {value.Read()}->std::same_as<bool>;
};

// 動的ディスパッチ用のフリー関数（実装は *_impl.hpp で提供）
void* AllocDigitalOutInstance(Pin pin);
void FreeDigitalOutInstance(void* inst);
void WriteDigitalOut(void* inst, bool state);
bool ReadDigitalOut(void* inst);

class DynDigitalOut {
 public:
  explicit DynDigitalOut(Pin pin) : instance_(AllocDigitalOutInstance(pin)) {}
  ~DynDigitalOut() { FreeDigitalOutInstance(instance_); }
  void Write(bool state) { WriteDigitalOut(instance_, state); }
  bool Read() { return ReadDigitalOut(instance_); }

 private:
  void* instance_;
};

static_assert(DigitalOut<DynDigitalOut>);

}  // namespace nano_hw
