# Project include, source and library dirs for uarm-related files
set(BKA_UARM_INC ${PROJECT_SOURCE_DIR}/${BKA_INC}/uarm)
set(BKA_UARM_SRC ${PROJECT_SOURCE_DIR}/${BKA_SRC}/uarm)

set(CFLAGS_UARM -nostdlib -O0 -mcpu=arm7tdmi)
set(LDFLAGS_UARM "-nostdlib -Wl,-G,0,-T,${BKA_UARM_SRC}/elf32ltsarm.h.uarmcore.x")

include_directories(${BKA_UARM_INC})


add_library(libuarm OBJECT ${BKA_UARM_SRC}/libuarm.s)
add_library(crtso OBJECT ${BKA_UARM_SRC}/crtso.s)
target_include_directories(libuarm PRIVATE ${BKA_UARM_INC})

add_executable(
	kernel0.uarm ${BKA_SRC}/phase0.c
	# Libraries the current target depends on
	${BKA_UARM_SRC}/libuarm.s ${BKA_UARM_SRC}/crtso.s ${BKA_SRC}/io.c
)
add_executable(
	kernel1.uarm ${BKA_SRC}/phase1.c
	# Libraries the current target depends on
	${BKA_UARM_SRC}/libuarm.s ${BKA_UARM_SRC}/crtso.s ${BKA_SRC}/io.c
	${BKA_SRC}/pcb.c ${BKA_SRC}/asl.c
)
add_executable(
	test.uarm ${BKA_SRC}/test.c
	# Libraries the current target depends on
	${BKA_UARM_SRC}/libuarm.s ${BKA_UARM_SRC}/crtso.s ${BKA_SRC}/io.c
	${BKA_SRC}/string.c ${BKA_SRC}/pcb.c ${BKA_SRC}/asl.c
)

set_property(TARGET kernel0.uarm PROPERTY COMPILE_OPTIONS ${CFLAGS_UARM})
set_property(TARGET kernel0.uarm PROPERTY LINK_FLAGS ${LDFLAGS_UARM})

set_property(TARGET kernel1.uarm PROPERTY COMPILE_OPTIONS ${CFLAGS_UARM})
set_property(TARGET kernel1.uarm PROPERTY LINK_FLAGS ${LDFLAGS_UARM})

set_property(TARGET test.uarm PROPERTY COMPILE_OPTIONS ${CFLAGS_UARM})
set_property(TARGET test.uarm PROPERTY LINK_FLAGS ${LDFLAGS_UARM})
