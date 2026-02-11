#pragma once

#include <concepts>

#include <NanoHW/pin.hpp>

namespace nano_hw {
template <typename T>
concept PwmOut = requires(T value, Pin pin, float duty_cycle, float period_s) {
  {T(pin)}->std::same_as<T>;
  {value.Write(duty_cycle)}->std::same_as<void>;
  {value.Read()}->std::same_as<float>;
  {value.SetPeriod(period_s)}->std::same_as<void>;
};

class DynPwmOut {
  void* AllocInstance(Pin pin);
  void FreeInstance(void* instance);

 public:
  explicit DynPwmOut(Pin pin) : instance_(AllocInstance(pin)) {}
  ~DynPwmOut() { FreeInstance(instance_); }

  void Write(float duty_cycle);
  float Read();
  void SetPeriod(float period_s);

 private:
  void* instance_;
};

static_assert(PwmOut<DynPwmOut>);

}  // namespace nano_hw
