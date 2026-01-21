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

class DynDigitalOut {
  void* AllocInstance(Pin pin);
  void FreeInstance(void* instance);

 public:
  explicit DynDigitalOut(Pin pin) : instance_(AllocInstance(pin)) {}
  void Write(bool state);
  bool Read();

 private:
  void* instance_;
};

static_assert(DigitalOut<DynDigitalOut>);

}  // namespace nano_hw
