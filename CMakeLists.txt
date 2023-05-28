cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE "cmake/i686-elf.cmake")

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

project(kernel C ASM)

include_directories(.)
add_executable(kernel main.c)

add_subdirectory(arch/i686)
add_subdirectory(drivers)
add_subdirectory(services)
add_subdirectory(third_party/printf)