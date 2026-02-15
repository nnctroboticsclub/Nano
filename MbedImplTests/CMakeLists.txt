cmake_minimum_required(VERSION 3.25)
cmake_policy(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use ARM toolchain for MbedImpl tests
set(MBED_APP_JSON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed_app.json)
set(MBED_TARGET "NUCLEO_F446RE" CACHE STRING "Mbed target")
set(ROBO_TOOLCHAIN_FILE ClangArmToolchain)

project(MbedImplTests)
enable_testing()
enable_language(C CXX ASM)

# Target
add_compile_options(-mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -mcpu=cortex-m4)
add_link_options(-mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mthumb -mcpu=cortex-m4)

# Debug
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -gfull -fstandalone-debug -gdwarf-5 -gz")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gfull -fstandalone-debug -gdwarf-5 -gz")

# Size reduction
add_compile_options(-Os)
add_compile_options(-ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Use parent directory's Nano build tree instead of installed version
set(NANO_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build")

find_package(StaticMbedOS-NUCLEO_F446RE REQUIRED)

# Import Nano targets directly from build directory
include("${NANO_BUILD_DIR}/NanoHW/NanoHWConfigVersion.cmake" OPTIONAL)
include("${CMAKE_CURRENT_SOURCE_DIR}/../NanoMbedImpl.cmake")

# Add Nano targets
add_library(Nano::NanoHW INTERFACE IMPORTED)
set_target_properties(Nano::NanoHW PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../NanoHW/include;${CMAKE_CURRENT_SOURCE_DIR}/../Nano/include"
)

# MbedImpl helper now available from included NanoMbedImpl.cmake
# Create MbedImpl target
nano_hw_mbed_impl(NanoHWImpl_Test StaticMbedOS-NUCLEO_F446RE)

# Add test subdirectory
add_subdirectory(tests)

add_library(NanoMbedImplUnit STATIC impl_unit.cpp)
target_link_libraries(NanoMbedImplUnit PRIVATE
  NanoHWImpl_Test
  Nano::NanoHW
)
