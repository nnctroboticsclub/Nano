#include <mbed.h>
#include <NanoHW/pwm.hpp>
#include <NanoMbedImpl/pwm.hpp>

using namespace nano_hw;

int main() {
  printf("\n=== MbedImpl PWM Test ===\n");

  // PWM pin for NUCLEO_F446RE
  // Use PA_8 (TIM1_CH1) for PWM output
  Pin pwm_pin = {PA_8};

  printf("Creating MbedPwmOut instance...\n");
  nano_mbed::MbedPwmOut pwm(pwm_pin);

  printf("Setting period to 20ms (50Hz)...\n");
  pwm.SetPeriod(0.02f);  // 20ms = 50Hz

  printf("Testing duty cycles:\n");

  for (float duty = 0.0f; duty <= 1.0f; duty += 0.25f) {
    printf("  Setting duty cycle to %.0f%%...\n", duty * 100.0f);
    pwm.Write(duty);

    float read_duty = pwm.Read();
    printf("  Read back: %.2f\n", read_duty);

    ThisThread::sleep_for(2s);
  }

  printf("\nTest complete!\n");

  while (true) {
    ThisThread::sleep_for(1s);
  }

  return 0;
}
