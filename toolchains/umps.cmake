# See https://cmake.org/cmake/help/v3.16/manual/cmake-toolchains.7.html#cross-compiling-for-linux
# for a description of what I'm doing here
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR mips)

# TODO Add find directive to locate other possible MIPS cross-compilers
set(CMAKE_C_COMPILER mipsel-linux-gnu-gcc)
set(CMAKE_ASM_COMPILER mipsel-linux-gnu-gcc)
set(CMAKE_C_LINKER mipsel-linux-gnu-ld)

# Skip compiler checks, which would otherwise fail. See
# https://stackoverflow.com/a/30642130/2069269
set(CMAKE_C_COMPILER_WORKS 1)
