# Project include, source and library dirs for uarm-related files
set(BKA_UARM_INC ${BKA_INC}/uarm)
set(BKA_UARM_SRC ${BKA_SRC}/uarm)

# Flags to be passed to the compiler
set(CFLAGS_UARM -nostdlib -mcpu=arm7tdmi -O0)
set(LINK_SCRIPT ${BKA_UARM_SRC}/elf32ltsarm.h.uarmcore.x)
# Flags to be passed to the linker. NOTE: the linker is not invoked directly,
# flags are passed to it via the -Wl compiler flag
set(LDFLAGS_UARM "-nostdlib -Wl,-nostdlib,-G,0,-T,${LINK_SCRIPT}")

# Set compiler and linker options
add_compile_options(${CFLAGS_UARM})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS_UARM}")
include_directories(${BKA_UARM_INC})

# uARM-dependent libraries
add_library(libuarm ${BKA_UARM_SRC}/libuarm.s)
add_library(libdiv ${BKA_UARM_SRC}/libdiv.s)
add_library(crtso ${BKA_UARM_SRC}/crtso.s)

# uARM executables
add_executable(test0.uarm ${BKA_TEST}/test0.c)
target_link_libraries(test0.uarm crtso libuarm io)

add_executable(test1.uarm ${BKA_TEST}/test1.c)
target_link_libraries(test1.uarm crtso libuarm libdiv asl io string)

add_executable(test2.uarm ${BKA_TEST}/test2.c)
target_link_libraries(test2.uarm crtso io string libuarm)

add_executable(unit_test.uarm ${BKA_TEST}/unit_test.c)
target_link_libraries(unit_test.uarm crtso libuarm libdiv asl io string)
