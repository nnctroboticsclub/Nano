cmake_minimum_required(VERSION 3.25)
cmake_policy(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ROBO_TOOLCHAIN_FILE ClangToolchain)

project(Nano)
enable_testing()
enable_language(C CXX ASM)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

function(add_nano_test test_name test_source)
  add_executable(${test_name} ${test_source})
  target_link_libraries(${test_name}
    PUBLIC
      Nano::Nano
      GTest::gtest
      GTest::gtest_main
  )
  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_subdirectory(MbedIF)
add_subdirectory(StubImpl)
add_subdirectory(Nano)
add_subdirectory(NanoHW)

# Add MbedImplTests as an ExternalProject (requires ARM toolchain)
option(BUILD_MBED_IMPL_TESTS "Build MbedImpl tests (requires ARM toolchain)" OFF)
if(BUILD_MBED_IMPL_TESTS)
  include(ExternalProject)

  # First, install Nano to a temporary location for MbedImplTests to use
  set(NANO_INSTALL_DIR ${CMAKE_BINARY_DIR}/nano_install)

  ExternalProject_Add(MbedImplTests
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MbedImplTests
    BINARY_DIR ${CMAKE_BINARY_DIR}/MbedImplTests-build
    CMAKE_ARGS
      -DCMAKE_PREFIX_PATH=${NANO_INSTALL_DIR}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    INSTALL_COMMAND ""
    DEPENDS NanoHW NanoHW_MbedIF
  )

  message(STATUS "MbedImplTests will be built with ARM toolchain")
  message(STATUS "  Note: These tests require actual hardware (NUCLEO_F446RE) to run")
endif()


# Install export
install(EXPORT NanoTargets
  FILE NanoTargets.cmake
  NAMESPACE Nano::
  DESTINATION lib/cmake/Nano
)

# Install MbedImpl helper
install(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/NanoMbedImpl.cmake
  DESTINATION lib/cmake/Nano
)

# Install MbedImpl sources
install(DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR}/MbedImpl/include/
  DESTINATION lib/cmake/Nano/MbedImpl/include
  FILES_MATCHING PATTERN "*.cpp" PATTERN "*.h" PATTERN "*.hpp"
)

# Set MbedImpl source paths for install
set(NANO_MBED_IMPL_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MbedImpl/source")

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