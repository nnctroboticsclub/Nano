#include <mbed.h>
#include <NanoHW/pwm_impl.hpp>

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

 private:
  mbed::PwmOut pwm_out_;
};

// PwmOutImpl をインスタンス化して Friend-Injection を有効化
template class nano_hw::PwmOutImpl<MbedPwmOut>;
