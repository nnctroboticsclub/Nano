add_library(MbedImplTestCommon INTERFACE)
target_compile_definitions(MbedImplTestCommon INTERFACE
  DEVICE_SERIAL=1
  DEVICE_SPI=1
  DEVICE_CAN=1
)

# Helper function to add MbedImpl test
function(add_mbed_impl_test test_name test_source)
  # Create executable with .elf suffix
  add_executable(${test_name} ${test_source})

  target_link_libraries(${test_name}
    PRIVATE
      NanoHWImpl_Test
      MbedImplTestCommon
      StaticMbedOS-NUCLEO_F446RE
  )

  # Generate .bin and .hex files
  static_mbed_os_nucleo_f446re_app_target(${test_name})

  # Note: These tests need to run on actual hardware
  # We can't run them in CI, but we can at least verify they compile
  message(STATUS "Created MbedImpl test: ${test_name} (requires hardware to run)")
endfunction()

# Add test sources
add_mbed_impl_test(Test_MbedImpl_SPI spi_test.cpp)
add_mbed_impl_test(Test_MbedImpl_UART uart_test.cpp)
add_mbed_impl_test(Test_MbedImpl_CAN can_test.cpp)
add_mbed_impl_test(Test_MbedImpl_Timer timer_test.cpp)
add_mbed_impl_test(Test_MbedImpl_PWM pwm_test.cpp)
add_mbed_impl_test(Test_MbedImpl_DigitalOut digital_out_test.cpp)
