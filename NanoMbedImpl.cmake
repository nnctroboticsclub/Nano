if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../../MbedImpl/include")
  set(MBED_IMPL_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../MbedImpl/include")
else()
  set(MBED_IMPL_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/MbedImpl/include")
endif()

# Function to create a Mbed implementation target linked to a specific mbed library
# Usage: nano_hw_mbed_impl(impl_target mbed_target)
#   impl_target: Name of the implementation target to create
#   mbed_target: Name of the mbed-os library target to link against
function(nano_hw_mbed_impl impl_target mbed_target)
  if(NOT TARGET ${mbed_target})
    message(FATAL_ERROR "nano_hw_mbed_impl: mbed_target '${mbed_target}' does not exist")
  endif()

  if(NOT TARGET Nano::NanoHW)
    message(FATAL_ERROR "nano_hw_mbed_impl: Nano::NanoHW is not available")
  endif()

  # Determine source directory (build tree vs install tree)

  # Create the implementation library with MbedImpl sources
  add_library(${impl_target} INTERFACE)

  # Link required libraries
  target_link_libraries(${impl_target} INTERFACE Nano::NanoHW)
  target_link_libraries(${impl_target} INTERFACE ${mbed_target})
  target_include_directories(${impl_target} INTERFACE ${MBED_IMPL_SOURCE_DIR})
  target_compile_features(${impl_target} INTERFACE cxx_std_20)

  # Add alias for consistent naming
  add_library(Nano::${impl_target} ALIAS ${impl_target})

  message(STATUS "Created Nano Mbed implementation target: ${impl_target} using ${mbed_target}")
endfunction()
