# CMake Toolchain File for ARM64 Cross-Compilation
# This tells CMake to use ARM compilers instead of x86 compilers

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Specify the cross compilers
# Adjust these paths based on your actual ARM compiler
# Common options: aarch64-linux-gnu-gcc, arm-linux-gnueabihf-gcc, or your custom gcarm

# Option 1: If gcarm is a wrapper for aarch64-linux-gnu-gcc
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Option 2: If you have a different ARM compiler, uncomment and modify:
# set(CMAKE_C_COMPILER /path/to/your/arm-gcc)
# set(CMAKE_CXX_COMPILER /path/to/your/arm-g++)

# Where to look for libraries
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a")
