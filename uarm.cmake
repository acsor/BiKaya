# Project include, source and library dirs for uarm-related files
set(BKA_UARM_INC ${PROJECT_SOURCE_DIR}/${BKA_INC}/uarm)
set(BKA_UARM_SRC ${PROJECT_SOURCE_DIR}/${BKA_SRC}/uarm)

set(CFLAGS_UARM -nostdlib -O0 -mcpu=arm7tdmi)
set(LDFLAGS_UARM "-nostdlib -Wl,-G,0,-T,${BKA_UARM_SRC}/elf32ltsarm.h.uarmcore.x")

include_directories(${BKA_UARM_INC})

add_executable(
	kernel0.uarm ${BKA_SRC}/phase0.c
	# Libraries the current target depends on
	${BKA_UARM_SRC}/libuarm.s ${BKA_UARM_SRC}/crtso.s ${BKA_SRC}/io.c
)

add_library(libuarm OBJECT ${BKA_UARM_SRC}/libuarm.s)
add_library(crtso OBJECT ${BKA_UARM_SRC}/crtso.s)
target_include_directories(libuarm PRIVATE ${BKA_UARM_INC})

add_custom_target(
	kernel1.uarm
	DEPENDS crtso libuarm io pcb asl phase1
	COMMAND ${CMAKE_C_LINKER} ${LDFLAGS_UARM}
	$<TARGET_OBJECTS:crtso> $<TARGET_OBJECTS:libuarm>
	$<TARGET_OBJECTS:io> $<TARGET_OBJECTS:phase1>
	$<TARGET_OBJECTS:pcb> $<TARGET_OBJECTS:asl>
	-o kernel1.uarm
)
set_property(TARGET kernel0.uarm PROPERTY COMPILE_OPTIONS ${CFLAGS_UARM})
set_property(TARGET kernel0.uarm PROPERTY LINK_FLAGS ${LDFLAGS_UARM})
