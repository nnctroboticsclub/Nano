add_library(NanoHW INTERFACE)
add_library(Nano::NanoHW ALIAS NanoHW)
target_include_directories(NanoHW INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/NanoHW>
)
target_link_libraries(NanoHW INTERFACE Nano)

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
