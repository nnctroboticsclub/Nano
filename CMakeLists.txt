cmake_minimum_required(VERSION 3.25)
cmake_policy(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ROBO_TOOLCHAIN_FILE ClangToolchain)

project(Nano)
enable_testing()
enable_language(C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# common sub-directories
add_subdirectory(Nano)
add_subdirectory(Mbed)
