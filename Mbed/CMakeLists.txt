add_library(Mbed INTERFACE)
add_library(Nano::Mbed ALIAS Mbed)
target_include_directories(Mbed INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/Nano/Mbed>
)

# Install targets
install(TARGETS Mbed
  EXPORT MbedTargets
  INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/
  DESTINATION include/Nano/Mbed
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Install export
install(EXPORT MbedTargets
  FILE MbedTargets.cmake
  NAMESPACE Nano::
  DESTINATION lib/cmake/Mbed
)

# Create config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/MbedConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/MbedConfig.cmake
  INSTALL_DESTINATION lib/cmake/Mbed
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/MbedConfig.cmake
  DESTINATION lib/cmake/Mbed
)