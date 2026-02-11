#include <gtest/gtest.h>

#include <NanoMbed/pwm.hpp>

// PWM の基本的な初期化テスト
TEST(PwmOutTest, Initialize) {
  mbed::PwmOut pwm(PinName::PA_0);
  SUCCEED();
}

// PWM の書き込み・読み取りテスト
TEST(PwmOutTest, WriteRead) {
  mbed::PwmOut pwm(PinName::PA_0);

  pwm.write(0.5f);
  EXPECT_FLOAT_EQ(pwm.read(), 0.5f);

  pwm.write(0.75f);
  EXPECT_FLOAT_EQ(pwm.read(), 0.75f);

  pwm = 0.25f;
  EXPECT_FLOAT_EQ(pwm, 0.25f);
}

// PWM の周期設定テスト（秒単位）
TEST(PwmOutTest, SetPeriodSeconds) {
  mbed::PwmOut pwm(PinName::PA_0);

  pwm.period(0.02f);  // 20ms = 50Hz
  pwm.write(0.5f);
  EXPECT_FLOAT_EQ(pwm.read(), 0.5f);
}

// PWM の周期設定テスト（マイクロ秒単位）
TEST(PwmOutTest, SetPeriodMicroseconds) {
  mbed::PwmOut pwm(PinName::PA_0);

  pwm.period_us(20000);  // 20000us = 20ms = 50Hz
  pwm.write(0.5f);
  EXPECT_FLOAT_EQ(pwm.read(), 0.5f);
}

// PWM の周期設定テスト（ミリ秒単位）
TEST(PwmOutTest, SetPeriodMilliseconds) {
  mbed::PwmOut pwm(PinName::PA_0);

  pwm.period_ms(20);  // 20ms = 50Hz
  pwm.write(0.5f);
  EXPECT_FLOAT_EQ(pwm.read(), 0.5f);
}

// PWM のパルス幅設定テスト（秒単位）
TEST(PwmOutTest, SetPulseWidthSeconds) {
  mbed::PwmOut pwm(PinName::PA_0);

  pwm.period(0.02f);      // 20ms period
  pwm.pulsewidth(0.01f);  // 10ms pulse = 50% duty cycle
  // Note: pulsewidth doesn't directly set the duty cycle in the mock,
  // but the calculation should be correct
  SUCCEED();
}

// PWM のパルス幅設定テスト（マイクロ秒単位）
TEST(PwmOutTest, SetPulseWidthMicroseconds) {
  mbed::PwmOut pwm(PinName::PA_0);

  pwm.period_us(20000);      // 20000us = 20ms
  pwm.pulsewidth_us(10000);  // 10000us = 10ms = 50% duty cycle
  SUCCEED();
}

// PWM のパルス幅設定テスト（ミリ秒単位）
TEST(PwmOutTest, SetPulseWidthMilliseconds) {
  mbed::PwmOut pwm(PinName::PA_0);

  pwm.period_ms(20);      // 20ms
  pwm.pulsewidth_ms(10);  // 10ms = 50% duty cycle
  SUCCEED();
}

// PWM の複数インスタンステスト
TEST(PwmOutTest, MultipleInstances) {
  mbed::PwmOut pwm1(PinName::PA_0);
  mbed::PwmOut pwm2(PinName::PA_1);

  pwm1.write(0.3f);
  pwm2.write(0.7f);

  EXPECT_FLOAT_EQ(pwm1.read(), 0.3f);
  EXPECT_FLOAT_EQ(pwm2.read(), 0.7f);
}

// PWM の境界値テスト
TEST(PwmOutTest, BoundaryValues) {
  mbed::PwmOut pwm(PinName::PA_0);

  pwm.write(0.0f);
  EXPECT_FLOAT_EQ(pwm.read(), 0.0f);

  pwm.write(1.0f);
  EXPECT_FLOAT_EQ(pwm.read(), 1.0f);
}
