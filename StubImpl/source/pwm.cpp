#include "NanoHW/pwm.hpp"
#include "NanoHW/pin.hpp"

#include <iostream>

class MockPwmOut {
 public:
  explicit MockPwmOut(nano_hw::Pin pin)
      : pin_(pin), duty_cycle_(0.0f), period_us_(20000) {}

  void Write(float duty_cycle) {
    duty_cycle_ = duty_cycle;
    std::cout << "PwmOut on pin " << pin_.number << " set to "
              << duty_cycle * 100.0f << "% duty cycle\n";
  }

  float Read() const {
    std::cout << "PwmOut on pin " << pin_.number << " read\n";
    return duty_cycle_;
  }

  void SetPeriod(float period_s) {
    period_us_ = static_cast<int>(period_s * 1e6f);
    std::cout << "PwmOut on pin " << pin_.number << " period set to "
              << period_s << "s\n";
  }

  void SetPeriodUs(int period_us) {
    period_us_ = period_us;
    std::cout << "PwmOut on pin " << pin_.number << " period set to "
              << period_us << "us\n";
  }

  void SetPeriodMs(int period_ms) {
    period_us_ = period_ms * 1000;
    std::cout << "PwmOut on pin " << pin_.number << " period set to "
              << period_ms << "ms\n";
  }

 private:
  nano_hw::Pin pin_;
  float duty_cycle_;
  int period_us_;
};

void* nano_hw::DynPwmOut::AllocInstance(Pin pin) {
  return new MockPwmOut(pin);
}

void nano_hw::DynPwmOut::FreeInstance(void* instance) {
  delete static_cast<MockPwmOut*>(instance);
}

void nano_hw::DynPwmOut::Write(float duty_cycle) {
  static_cast<MockPwmOut*>(instance_)->Write(duty_cycle);
}

float nano_hw::DynPwmOut::Read() {
  return static_cast<MockPwmOut*>(instance_)->Read();
}

void nano_hw::DynPwmOut::SetPeriod(float period_s) {
  static_cast<MockPwmOut*>(instance_)->SetPeriod(period_s);
}
