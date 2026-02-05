# MbedImpl is a template implementation that gets instantiated via nano_hw_mbed_impl()
# Sources are distributed and compiled when the function is called
# This allows linking against different mbed-os targets

# Export source file list for use in parent CMake
set(NANO_MBED_IMPL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/source/digital_out.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/can.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/spi.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/uart.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/thread.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/rtos.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/timer.cpp
  PARENT_SCOPE
)
