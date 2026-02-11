#pragma once

#include "NanoHW/pin.hpp"
#include "NanoHW/pwm_impl.hpp"

#include <iostream>

namespace nano_stub {
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

 private:
  nano_hw::Pin pin_;
  float duty_cycle_;
  int period_us_;
};
}  // namespace nano_stub