# Project include, source and library dirs for uarm-related files
set(BKA_UARM_INC ${PROJECT_SOURCE_DIR}/${BKA_INC}/uarm)
set(BKA_UARM_SRC ${PROJECT_SOURCE_DIR}/${BKA_SRC}/uarm)

set(CFLAGS_UARM -nostdlib -O0 -mcpu=arm7tdmi)
set(LDFLAGS_UARM "-nostdlib -Wl,-G,0,-T,${BKA_UARM_SRC}/elf32ltsarm.h.uarmcore.x")

include_directories(${BKA_UARM_INC})

add_executable(
	kernel.uarm ${BKA_SRC}/phase0.c
	# Libraries the current target depends on
	${BKA_UARM_SRC}/libuarm.s ${BKA_UARM_SRC}/crtso.s ${BKA_SRC}/io.c
)
set_property(TARGET kernel.uarm PROPERTY COMPILE_OPTIONS ${CFLAGS_UARM})
set_property(TARGET kernel.uarm PROPERTY LINK_FLAGS ${LDFLAGS_UARM})
