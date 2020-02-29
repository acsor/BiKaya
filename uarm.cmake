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

# Dependencies are not passed as libraries, but directly as source files
set(libuarm ${BKA_UARM_SRC}/libuarm.s)
set(libdiv ${BKA_UARM_SRC}/libdiv.s)
set(crtso ${BKA_UARM_SRC}/crtso.s)

add_executable(
    kernel0.uarm ${BKA_SRC}/phase0.c ${crtso} ${libuarm} ${BKA_SRC}/io.c
)
add_executable(
    kernel1.uarm ${BKA_SRC}/phase1.c ${crtso} ${libuarm}
    ${libdiv} ${BKA_SRC}/pcb.c ${BKA_SRC}/asl.c
    ${BKA_SRC}/io.c ${BKA_SRC}/math.c ${BKA_SRC}/string.c ${BKA_SRC}/utils.c
)
add_executable(
    bka_test.uarm ${BKA_SRC}/bka_test.c ${crtso} ${libuarm}
    ${libdiv} ${BKA_SRC}/pcb.c ${BKA_SRC}/asl.c
    ${BKA_SRC}/io.c ${BKA_SRC}/math.c ${BKA_SRC}/string.c ${BKA_SRC}/utils.c
)
