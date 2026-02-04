add_library(NanoHW STATIC src/can.cpp)
add_library(Nano::NanoHW ALIAS NanoHW)
target_include_directories(NanoHW PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/NanoHW>
)

# Install targets
install(TARGETS NanoHW
  EXPORT NanoTargets
  INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/
  DESTINATION include/NanoHW
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
