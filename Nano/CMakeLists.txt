cmake_minimum_required(VERSION 3.20)
project(Nano VERSION 1.0.0 LANGUAGES CXX)

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

find_package(GTest REQUIRED)

add_executable(NanoTest_NoMutexLIFO tests/test_no_mutex_lifo.cpp)
target_link_libraries(NanoTest_NoMutexLIFO
  PUBLIC
    Nano::Nano
    GTest::gtest
    GTest::gtest_main
)
add_test(NAME NanoTest_NoMutexLIFO COMMAND NanoTest_NoMutexLIFO)

add_executable(NanoTest_Span tests/test_span.cpp)
target_link_libraries(NanoTest_Span
  PUBLIC
    Nano::Nano
    GTest::gtest
    GTest::gtest_main
)
add_test(NAME NanoTest_Span COMMAND NanoTest_Span)

add_executable(NanoTest_Singleton tests/test_singleton.cpp)
target_link_libraries(NanoTest_Singleton
  PUBLIC
    Nano::Nano
    GTest::gtest
    GTest::gtest_main
)
add_test(NAME NanoTest_Singleton COMMAND NanoTest_Singleton)

add_executable(NanoTest_ManagedList tests/test_managed_list.cpp)
target_link_libraries(NanoTest_ManagedList
  PUBLIC
    Nano::Nano
    GTest::gtest
    GTest::gtest_main
)
add_test(NAME NanoTest_ManagedList COMMAND NanoTest_ManagedList)

add_executable(NanoTest_FixedMap tests/test_fixed_map.cpp)
target_link_libraries(NanoTest_FixedMap
  PUBLIC
    Nano::Nano
    GTest::gtest
    GTest::gtest_main
)
add_test(NAME NanoTest_FixedMap COMMAND NanoTest_FixedMap)

add_executable(NanoTest_LinkedList tests/test_linked_list.cpp)
target_link_libraries(NanoTest_LinkedList
  PUBLIC
    Nano::Nano
    GTest::gtest
    GTest::gtest_main
)
add_test(NAME NanoTest_LinkedList COMMAND NanoTest_LinkedList)