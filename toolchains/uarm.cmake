# See https://cmake.org/cmake/help/v3.16/manual/cmake-toolchains.7.html#cross-compiling-for-linux
# for a description of what I'm doing here
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# TODO Add find directive to locate other possible ARM cross-compilers

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_LINKER arm-none-eabi-ld)
# See https://stackoverflow.com/q/43781207/
set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")

# Skip compiler checks, which would otherwise fail. See
# https://stackoverflow.com/a/30642130/2069269
set(CMAKE_C_COMPILER_WORKS 1)
