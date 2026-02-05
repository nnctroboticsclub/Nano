#include <mbed.h>
#include <NanoHW/digital_out.hpp>

class MbedDigitalOut {
 public:
  explicit MbedDigitalOut(nano_hw::Pin pin)
      : digital_out_(static_cast<PinName>(pin.number)) {}

  void Write(bool state) { digital_out_.write(state ? 1 : 0); }

  bool Read() const {
    // Cast away const to call non-const read() method
    return const_cast<mbed::DigitalOut&>(digital_out_).read() != 0;
  }

 private:
  mbed::DigitalOut digital_out_;
};

void* nano_hw::DynDigitalOut::AllocInstance(Pin pin) {
  return new MbedDigitalOut(pin);
}

void nano_hw::DynDigitalOut::FreeInstance(void* instance) {
  delete static_cast<MbedDigitalOut*>(instance);
}

void nano_hw::DynDigitalOut::Write(bool state) {
  static_cast<MbedDigitalOut*>(instance_)->Write(state);
}

bool nano_hw::DynDigitalOut::Read() {
  return static_cast<MbedDigitalOut*>(instance_)->Read();
}
