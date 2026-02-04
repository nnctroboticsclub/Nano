#include <gtest/gtest.h>
#include <NanoMbed/can.hpp>

using mbed::CAN;
using mbed::CANMessage;

// CANの初期化と基本的な送信テスト
TEST(CANTest, InitializeAndSendMessage) {
  // StubImplを使用してCANを初期化
  CAN can(mbed::NC, mbed::NC);  // StubImplはダミーピンを受け付ける

  // テスト用のCANメッセージを作成
  uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  CANMessage msg(0x123, test_data, 8);

  // メッセージを送信
  int result = can.write(msg);

  // 送信結果の確認
  EXPECT_EQ(result, 1);  // 正常な送信は1を返す
}

// CANメッセージの属性確認テスト
TEST(CANTest, CANMessageProperties) {
  uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
  CANMessage msg(0x456, data, 4);

  EXPECT_EQ(msg.id, 0x456);
  EXPECT_EQ(msg.len, 4);
  EXPECT_EQ(msg.data[0], 0xAA);
  EXPECT_EQ(msg.data[1], 0xBB);
  EXPECT_EQ(msg.data[2], 0xCC);
  EXPECT_EQ(msg.data[3], 0xDD);
}

// CANリセット機能のテスト
TEST(CANTest, ResetPeripherals) {
  CAN can(mbed::NC, mbed::NC);

  // リセットしても例外が発生しないことを確認
  can.reset();
  SUCCEED();
}

// CANエラーカウンタのテスト
TEST(CANTest, ErrorCounters) {
  CAN can(mbed::NC, mbed::NC);

  // StubImplではエラーカウンタは常に0を返す
  int rx_errors = can.rderror();
  int tx_errors = can.tderror();

  EXPECT_EQ(rx_errors, 0);
  EXPECT_EQ(tx_errors, 0);
}

// CANメッセージの複数送信テスト
TEST(CANTest, MultipleSendMessages) {
  CAN can(mbed::NC, mbed::NC);

  // 複数のメッセージを送信
  for (uint32_t id = 0x100; id < 0x110; ++id) {
    uint8_t data[] = {static_cast<uint8_t>(id & 0xFF)};
    CANMessage msg(id, data, 1);
    int result = can.write(msg);
    EXPECT_EQ(result, 1);
  }
}

// CANメッセージの異なるデータサイズのテスト
TEST(CANTest, DifferentDataSizes) {
  CAN can(mbed::NC, mbed::NC);

  // 1バイト
  {
    uint8_t data[] = {0x11};
    CANMessage msg(0x111, data, 1);
    EXPECT_EQ(can.write(msg), 1);
  }

  // 4バイト
  {
    uint8_t data[] = {0x11, 0x22, 0x33, 0x44};
    CANMessage msg(0x222, data, 4);
    EXPECT_EQ(can.write(msg), 1);
  }

  // 8バイト（最大）
  {
    uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    CANMessage msg(0x333, data, 8);
    EXPECT_EQ(can.write(msg), 1);
  }
}

// CANメッセージの拡張IDテスト
TEST(CANTest, ExtendedIDMessage) {
  uint8_t data[] = {0x01, 0x02};
  CANMessage msg(0x12345678, data, 2);  // 29-bit extended ID

  EXPECT_EQ(msg.id, 0x12345678);
  EXPECT_EQ(msg.len, 2);
}

// CANメッセージの変換テスト（mbed <-> nano_hw）
TEST(CANTest, MessageConversion) {
  uint8_t data[] = {0x10, 0x20, 0x30, 0x40};
  CANMessage mbed_msg(0x789, data, 4);

  // mbed::CANMessage -> nano_hw::can::CANMessage
  nano_hw::can::CANMessage nano_msg =
      static_cast<nano_hw::can::CANMessage>(mbed_msg);

  EXPECT_EQ(nano_msg.id, 0x789);
  EXPECT_EQ(nano_msg.len, 4);
  EXPECT_EQ(nano_msg.data[0], 0x10);
  EXPECT_EQ(nano_msg.data[1], 0x20);

  // nano_hw::can::CANMessage -> mbed::CANMessage
  CANMessage converted_back = CANMessage::from_nano_hw(nano_msg);

  EXPECT_EQ(converted_back.id, 0x789);
  EXPECT_EQ(converted_back.len, 4);
  EXPECT_EQ(converted_back.data[0], 0x10);
  EXPECT_EQ(converted_back.data[1], 0x20);
}

// CANフィルタ設定のテスト
TEST(CANTest, FilterConfiguration) {
  CAN can(mbed::NC, mbed::NC);

  mbed::CAN_FilterConfTypeDef filter_config;
  filter_config.FilterNumber = 0;
  filter_config.FilterMode = mbed::CAN_FILTERMODE_IDMASK;
  filter_config.FilterScale = mbed::CAN_FILTERSCALE_32BIT;
  filter_config.FilterIdHigh = 0x0001;
  filter_config.FilterIdLow = 0x0000;
  filter_config.FilterMaskIdHigh = 0xFFFF;
  filter_config.FilterMaskIdLow = 0x0000;
  filter_config.FilterFIFOAssignment = mbed::CAN_FILTER_FIFO0;
  filter_config.FilterActivation = ENABLE;

  mbed::HAL_CAN_TypeDef hal_can;
  hal_can.can = &can;

  // フィルタ設定の実行
  HAL_StatusTypeDef status = HAL_CAN_ConfigFilter(&hal_can, &filter_config);

  EXPECT_EQ(status, HAL_OK);
}

// CANスタンダードIDメッセージのテスト
TEST(CANTest, StandardIDMessage) {
  uint8_t data[] = {0x55, 0x66, 0x77, 0x88};
  CANMessage msg(0x7FF, data, 4);  // 最大11-bit standard ID

  EXPECT_EQ(msg.id, 0x7FF);
  EXPECT_EQ(msg.len, 4);
}

// CANメッセージコンストラクタのテスト
TEST(CANTest, DefaultConstructor) {
  CANMessage msg;

  EXPECT_EQ(msg.id, 0);
  EXPECT_EQ(msg.len, 0);
}

// CANメッセージのデータフィールドアクセステスト
TEST(CANTest, DataFieldAccess) {
  CANMessage msg;
  msg.id = 0x222;
  msg.len = 3;
  msg.data[0] = 0xAA;
  msg.data[1] = 0xBB;
  msg.data[2] = 0xCC;

  EXPECT_EQ(msg.id, 0x222);
  EXPECT_EQ(msg.len, 3);
  EXPECT_EQ(msg.data[0], 0xAA);
  EXPECT_EQ(msg.data[1], 0xBB);
  EXPECT_EQ(msg.data[2], 0xCC);
}
