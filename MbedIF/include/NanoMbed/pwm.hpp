#pragma once

#include <NanoHW/pwm.hpp>

#include "pin_name.hpp"

namespace {
namespace mbed {
class PwmOut {
 public:
  explicit(false) PwmOut(PinName pin) : dri_(ToPin(pin)) {}

  void write(float duty_cycle) { dri_.Write(duty_cycle); }
  float read() { return dri_.Read(); }

  void period(float period_s) {
    current_period_ = period_s;
    dri_.SetPeriod(period_s);
  }
  void period_us(int period_us) {
    period(static_cast<float>(period_us) / 1e6f);
  }
  void period_ms(int period_ms) {
    period(static_cast<float>(period_ms) / 1e3f);
  }

  void pulsewidth(float pulse_width_s) {
    write(pulse_width_s / current_period_);
  }
  void pulsewidth_us(int pulse_width_us) {
    write(static_cast<float>(pulse_width_us) / (current_period_ * 1e6f));
  }
  void pulsewidth_ms(int pulse_width_ms) {
    write(static_cast<float>(pulse_width_ms) / (current_period_ * 1e3f));
  }

  operator float() { return read(); }
  PwmOut& operator=(float value) {
    write(value);
    return *this;
  }

 private:
  nano_hw::DynPwmOut dri_;
  float current_period_ = 0.02f;  // Default 20ms (50Hz)
};

}  // namespace mbed
}  // namespace
