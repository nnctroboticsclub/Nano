#include <NanoHW/can_impl.hpp>
#include <NanoHW/digital_out_impl.hpp>
#include <NanoHW/high_res_clock_impl.hpp>
#include <NanoHW/parallel.hpp>
#include <NanoHW/pwm_impl.hpp>
#include <NanoHW/spi_impl.hpp>
#include <NanoHW/system_impl.hpp>
#include <NanoHW/thread_impl.hpp>
#include <NanoHW/timer_impl.hpp>
#include <NanoHW/uart_impl.hpp>

#include <NanoMbedImpl/can.hpp>
#include <NanoMbedImpl/digital_out.hpp>
#include <NanoMbedImpl/high_res_clock.hpp>
#include <NanoMbedImpl/pwm.hpp>
#include <NanoMbedImpl/rtos.hpp>
#include <NanoMbedImpl/spi.hpp>
#include <NanoMbedImpl/system.hpp>
#include <NanoMbedImpl/thread.hpp>
#include <NanoMbedImpl/timer.hpp>
#include <NanoMbedImpl/uart.hpp>

using nano_mbed::MbedCAN;
using nano_mbed::MbedDigitalOut;
using nano_mbed::MbedHighResClock;
using nano_mbed::MbedPwmOut;
using nano_mbed::MbedSleepManager;
using nano_mbed::MbedSPI;
using nano_mbed::MbedThread;
using nano_mbed::MbedTimer;
using nano_mbed::MbedUART;

template struct nano_hw::can::CANImpl<MbedCAN>;
template struct nano_hw::DigitalOutImpl<MbedDigitalOut>;
template struct nano_hw::HighResClockImpl<MbedHighResClock>;
template struct nano_hw::PwmOutImpl<MbedPwmOut>;
template struct nano_hw::spi::SPIImpl<MbedSPI>;
template struct nano_hw::SleepManagerImpl<MbedSleepManager>;
template struct nano_hw::thread::ThreadImpl<MbedThread>;
template struct nano_hw::timer::TimerImpl<MbedTimer>;
template struct nano_hw::uart::UARTImpl<MbedUART>;
