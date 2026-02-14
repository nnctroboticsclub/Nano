#include <mbed.h>
#include <NanoHW/can.hpp>
#include <NanoMbedImpl/can.hpp>

using namespace nano_hw;
using namespace nano_hw::can;

// Test CAN callbacks implementation
class TestCANCallbacks : public ICallbacks {
 public:
  void OnCANReceived(void* ctx, nano_hw::can::CANMessage msg) override {
    (void)ctx;
    printf("CAN RX: ID=0x%03x, len=%d\n", msg.id, msg.len);
    for (int i = 0; i < msg.len; ++i) {
      printf("  data[%d] = 0x%02x\n", i, msg.data[i]);
    }
  }

  void OnCANTransmit(void* ctx, nano_hw::can::CANMessage msg) override {
    (void)ctx;
    printf("CAN TX: ID=0x%03x, len=%d sent\n", msg.id, msg.len);
  }

  void OnCANBusError(void* ctx) override {
    (void)ctx;
    printf("CAN Bus Error!\n");
  }

  void OnCANPassiveError(void* ctx) override {
    (void)ctx;
    printf("CAN Passive Error!\n");
  }
};

int main() {
  printf("\n=== MbedImpl CAN Test ===\n");

  // CAN pins for NUCLEO_F446RE
  // CAN1: RX=PA_11, TX=PA_12
  Pin tx = {PA_12};
  Pin rx = {PA_11};

  TestCANCallbacks callbacks;
  void* callback_ctx = nullptr;

  printf("Creating MbedCAN instance...\n");
  nano_mbed::MbedCAN<nano_hw::can::DummyCANConfig> can(
      tx, rx, 500000, callback_ctx);

  printf("Setting to normal mode...\n");
  can.ChangeMode(CANMode::kNormal);

  printf("Sending test CAN message...\n");
  nano_hw::can::CANMessage test_msg;
  test_msg.id = 0x123;
  test_msg.len = 8;
  for (int i = 0; i < 8; ++i) {
    test_msg.data[i] = i + 1;
  }

  if (can.SendMessage(test_msg)) {
    printf("Message sent successfully!\n");
  } else {
    printf("Failed to send message!\n");
  }

  printf("\nWaiting for incoming CAN messages...\n");
  for (int i = 0; i < 10; ++i) {
    ThisThread::sleep_for(1s);
    printf("Waiting... %d\n", i + 1);
  }

  printf("\nTest complete!\n");

  while (true) {
    ThisThread::sleep_for(1s);
  }

  return 0;
}
