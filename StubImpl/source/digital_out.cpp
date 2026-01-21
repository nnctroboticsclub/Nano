#include "NanoHW/digital_out.hpp"
#include <iostream>
#include "NanoHW/pin.hpp"

class MockDigitalOut {
 public:
  MockDigitalOut(nano_hw::Pin pin) : pin_(pin) {}

  void Write(bool state) {
    std::cout << "DigitalOut on pin " << pin_.number << " set to " << state
              << "\n";
  }

  bool Read() {
    std::cout << "DigitalOut on pin " << pin_.number << " read\n";
    return false;  // Mock always returns false
  }

 private:
  nano_hw::Pin pin_;
};

void* nano_hw::DynDigitalOut::AllocInstance(Pin pin) {
  return new MockDigitalOut(pin);
}
void nano_hw::DynDigitalOut::FreeInstance(void* instance) {
  delete static_cast<MockDigitalOut*>(instance);
}
void nano_hw::DynDigitalOut::Write(bool state) {
  static_cast<MockDigitalOut*>(instance_)->Write(state);
}
bool nano_hw::DynDigitalOut::Read() {
  return static_cast<MockDigitalOut*>(instance_)->Read();
}