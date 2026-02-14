#include <mbed.h>
#include <NanoHW/digital_out.hpp>
#include <NanoMbedImpl/digital_out.hpp>

using namespace nano_hw;

int main() {
  printf("\n=== MbedImpl DigitalOut Test ===\n");

  // Use onboard LED (PA_5 on NUCLEO_F446RE = LD2)
  Pin led_pin = {LED1};

  printf("Creating MbedDigitalOut instance...\n");
  nano_mbed::MbedDigitalOut led(led_pin);

  printf("Blinking LED 10 times...\n");

  for (int i = 0; i < 10; ++i) {
    printf("  Iteration %d: LED ON\n", i + 1);
    led.Write(true);
    ThisThread::sleep_for(500ms);

    bool state = led.Read();
    printf("  Read state: %s\n", state ? "HIGH" : "LOW");

    printf("  Iteration %d: LED OFF\n", i + 1);
    led.Write(false);
    ThisThread::sleep_for(500ms);
  }

  printf("\nTest complete!\n");

  while (true) {
    ThisThread::sleep_for(1s);
  }

  return 0;
}
