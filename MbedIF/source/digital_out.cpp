#include <NanoHW/digital_out.hpp>
#include <NanoMbed/pin_name.hpp>

namespace {
mbed::PinName ToPinName(nano_hw::Pin pin) {
  // マッピングロジックをここに実装
  return static_cast<mbed::PinName>(pin.number);
}
}  // namespace

// MbedImpl's function
// void* nano_hw::DynDigitalOut::AllocInstance(Pin pin) {
//   return new mbed::DigitalOut(static_cast<mbed::PinName>(pin));
// }
