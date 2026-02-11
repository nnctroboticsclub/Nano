#include <mbed.h>
#include <NanoHW/pwm.hpp>

class MbedPwmOut {
 public:
  explicit MbedPwmOut(nano_hw::Pin pin)
      : pwm_out_(static_cast<PinName>(pin.number)) {}

  void Write(float duty_cycle) { pwm_out_.write(duty_cycle); }

  float Read() const {
    // Cast away const to call non-const read() method
    return const_cast<mbed::PwmOut&>(pwm_out_).read();
  }

  void SetPeriod(float period_s) { pwm_out_.period(period_s); }

  void SetPeriodUs(int period_us) { pwm_out_.period_us(period_us); }

  void SetPeriodMs(int period_ms) { pwm_out_.period_ms(period_ms); }

 private:
  mbed::PwmOut pwm_out_;
};

void* nano_hw::DynPwmOut::AllocInstance(Pin pin) {
  return new MbedPwmOut(pin);
}

void nano_hw::DynPwmOut::FreeInstance(void* instance) {
  delete static_cast<MbedPwmOut*>(instance);
}

void nano_hw::DynPwmOut::Write(float duty_cycle) {
  static_cast<MbedPwmOut*>(instance_)->Write(duty_cycle);
}

float nano_hw::DynPwmOut::Read() {
  return static_cast<MbedPwmOut*>(instance_)->Read();
}

void nano_hw::DynPwmOut::SetPeriod(float period_s) {
  static_cast<MbedPwmOut*>(instance_)->SetPeriod(period_s);
}
