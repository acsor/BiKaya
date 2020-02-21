# Project include, source and library dirs for uarm-related files
set(BKA_UARM_INC ${PROJECT_SOURCE_DIR}/${BKA_INC}/uarm)
set(BKA_UARM_SRC ${PROJECT_SOURCE_DIR}/${BKA_SRC}/uarm)

set(CFLAGS_UARM -mcpu=arm7tdmi)
set(LDFLAGS_UARM -G 0 -nostdlib -T ${BKA_UARM_SRC}/elf32ltsarm.h.uarmcore.x)

add_compile_options(${CFLAGS_UARM})


add_library(libuarm OBJECT ${BKA_UARM_SRC}/libuarm.s)
add_library(crtso OBJECT ${BKA_UARM_SRC}/crtso.s)

target_include_directories(libuarm PRIVATE ${BKA_UARM_INC})

add_custom_target(
	kernel0.uarm
	DEPENDS crtso libuarm io phase0
	COMMAND ${CMAKE_C_LINKER} ${LDFLAGS_UARM}
	$<TARGET_OBJECTS:crtso> $<TARGET_OBJECTS:libuarm>
	$<TARGET_OBJECTS:io> $<TARGET_OBJECTS:phase0>
	-o kernel.uarm
)
add_custom_target(
	kernel1.uarm
	DEPENDS crtso libuarm pcb asl io phase1 
	COMMAND ${CMAKE_C_LINKER} ${LDFLAGS_UARM}
	$<TARGET_OBJECTS:crtso> $<TARGET_OBJECTS:libuarm>
	$<TARGET_OBJECTS:pcb> $<TARGET_OBJECTS:asl>
	$<TARGET_OBJECTS:io> $<TARGET_OBJECTS:phase1>
	-o kernel1.uarm
)
add_custom_target(
	bka_test.uarm
	DEPENDS crtso libuarm io math string utils _bka_test
	COMMAND ${CMAKE_C_LINKER} ${LDFLAGS_UARM}
	$<TARGET_OBJECTS:crtso> $<TARGET_OBJECTS:libuarm>
	$<TARGET_OBJECTS:io> $<TARGET_OBJECTS:math> $<TARGET_OBJECTS:string>
	$<TARGET_OBJECTS:utils> $<TARGET_OBJECTS:_bka_test>
	-o bka_test.uarm
)
