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

// Forward declarations for dynamic dispatch functions
void* AllocPwmOutInstance(Pin pin);
void FreePwmOutInstance(void* inst);
void WritePwmOut(void* inst, float duty_cycle);
float ReadPwmOut(void* inst);
void SetPeriodPwmOut(void* inst, float period_s);

class DynPwmOut {
 public:
  explicit DynPwmOut(Pin pin) : instance_(AllocPwmOutInstance(pin)) {}
  ~DynPwmOut() { FreePwmOutInstance(instance_); }

  void Write(float duty_cycle) { WritePwmOut(instance_, duty_cycle); }
  float Read() { return ReadPwmOut(instance_); }
  void SetPeriod(float period_s) { SetPeriodPwmOut(instance_, period_s); }

 private:
  void* instance_;
};

static_assert(PwmOut<DynPwmOut>);

}  // namespace nano_hw
