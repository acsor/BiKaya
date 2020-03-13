# Project include, source and library dirs for uarm-related files
set(BKA_UARM_INC ${PROJECT_SOURCE_DIR}/${BKA_INC}/uarm)
set(BKA_UARM_SRC ${PROJECT_SOURCE_DIR}/${BKA_SRC}/uarm)

# Flags to be passed to the compiler
set(CFLAGS_UARM -mcpu=arm7tdmi -O0)
set(LINK_SCRIPT ${BKA_UARM_SRC}/elf32ltsarm.h.uarmcore.x)
# Flags to be passed to the linker. NOTE: the linker is not invoked directly,
# flags are passed to it via the -Wl compiler flag
set(LDFLAGS_UARM "-Wl,-nostdlib,-G,0,-T,${LINK_SCRIPT}")

# Set compiler and linker options
add_compile_options(${CFLAGS_UARM})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS_UARM}")
include_directories(${BKA_UARM_INC})

# uARM-dependent libraries
add_library(libuarm ${BKA_UARM_SRC}/libuarm.s)
add_library(libdiv ${BKA_UARM_SRC}/libdiv.s)
add_library(crtso ${BKA_UARM_SRC}/crtso.s)

# uARM executables
add_executable(kernel0.uarm ${BKA_SRC}/phase0.c)
target_link_libraries(kernel0.uarm crtso libuarm io)

add_executable(kernel1.uarm ${BKA_SRC}/phase1.c)
target_link_libraries(kernel1.uarm crtso libuarm libdiv pcb asl io math string utils)

add_executable(bka_test.uarm ${BKA_SRC}/bka_test.c)
target_link_libraries(bka_test.uarm crtso libuarm libdiv pcb asl io math string utils)
