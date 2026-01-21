add_library(NanoHW_MbedIF STATIC
  source/digital_out.cpp
)
add_library(Nano::NanoHW_MbedIF ALIAS NanoHW_MbedIF)
target_include_directories(NanoHW_MbedIF PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/Nano/NanoHW_MbedIF>
)
target_link_libraries(NanoHW_MbedIF PUBLIC Nano::NanoHW)

# Install targets
install(TARGETS NanoHW_MbedIF
  EXPORT NanoTargets
  INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/
  DESTINATION include/Nano/NanoHW_MbedIF
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
