add_library(NanoHW_MbedIF INTERFACE)
add_library(Nano::NanoHW_MbedIF ALIAS NanoHW_MbedIF)
target_include_directories(NanoHW_MbedIF INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/NanoHW/MbedIF>
)
target_link_libraries(NanoHW_MbedIF INTERFACE Nano::NanoHW)

# Install targets
install(TARGETS NanoHW_MbedIF
  EXPORT NanoTargets
  INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/
  DESTINATION include/NanoHW/MbedIF
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)


add_nano_test(Test_NanoHW_MbedIF_CAN tests/can.cpp)
target_link_libraries(Test_NanoHW_MbedIF_CAN PUBLIC Nano::NanoHW_MbedIF Nano::NanoHW_StubImpl)

add_nano_test(Test_NanoHW_MbedIF_SPI tests/spi.cpp)
target_link_libraries(Test_NanoHW_MbedIF_SPI PUBLIC Nano::NanoHW_MbedIF Nano::NanoHW_StubImpl)

add_nano_test(Test_NanoHW_MbedIF_UART tests/uart.cpp)
target_link_libraries(Test_NanoHW_MbedIF_UART PUBLIC Nano::NanoHW_MbedIF Nano::NanoHW_StubImpl)

add_nano_test(Test_NanoHW_MbedIF_Thread tests/thread.cpp)
target_link_libraries(Test_NanoHW_MbedIF_Thread PUBLIC Nano::NanoHW_MbedIF Nano::NanoHW_StubImpl)
