add_library(NanoHW_StubImpl STATIC
  source/digital_out.cpp
  source/can.cpp
  source/spi.cpp
  source/uart.cpp
  source/thread.cpp
  source/rtos.cpp
  source/timer.cpp
  source/system.cpp
  source/high_res_clock.cpp
)
add_library(Nano::NanoHW_StubImpl ALIAS NanoHW_StubImpl)
target_link_libraries(NanoHW_StubImpl PUBLIC Nano::NanoHW)

install(TARGETS NanoHW_StubImpl EXPORT NanoTargets)
