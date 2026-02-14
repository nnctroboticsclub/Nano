#include <mbed.h>
#include <NanoHW/uart.hpp>
#include <NanoMbedImpl/uart.hpp>

using namespace nano_hw;
using namespace nano_hw::uart;

// Test configuration
struct TestConfig {
  struct OnUARTRx {
    static void execute(void* ctx, const uint8_t* data, size_t len) {
      (void)ctx;
      printf("UART RX: %zu bytes\n", len);
      for (size_t i = 0; i < len; ++i) {
        printf("  rx[%zu] = 0x%02x ('%c')\n", i, data[i],
               isprint(data[i]) ? data[i] : '.');
      }
    }
  };

  struct OnUARTTx {
    static void execute(void* ctx, const uint8_t* data, size_t len) {
      (void)ctx;
      (void)data;
      printf("UART TX: %zu bytes sent\n", len);
    }
  };
};

int main() {
  printf("\n=== MbedImpl UART Test ===\n");

  // UART pins for NUCLEO_F446RE
  // USART2: TX=PA_2, RX=PA_3 (connected to ST-LINK USB)
  Pin tx = {PA_2};
  Pin rx = {PA_3};

  printf("Creating MbedUART instance...\n");
  nano_mbed::MbedUART<TestConfig> uart(tx, rx, 115200);

  printf("Sending test message...\n");
  const char* test_msg = "Hello from MbedUART!\n";
  uart.Send(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(test_msg)),
            strlen(test_msg));

  printf("\nWaiting for incoming data (type something in terminal)...\n");

  // Let the UART thread process incoming data
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
