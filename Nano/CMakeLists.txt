add_library(Nano INTERFACE)
add_library(Nano::Nano ALIAS Nano)
target_include_directories(Nano INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/Nano>
)

# Install targets
install(TARGETS Nano
  EXPORT NanoTargets
  INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/
  DESTINATION include/Nano
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Install export
install(EXPORT NanoTargets
  FILE NanoTargets.cmake
  NAMESPACE Nano::
  DESTINATION lib/cmake/Nano
)

# Create config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/NanoConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/NanoConfig.cmake
  INSTALL_DESTINATION lib/cmake/Nano
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/NanoConfig.cmake
  DESTINATION lib/cmake/Nano
)