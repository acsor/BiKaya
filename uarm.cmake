# Project include, source and library dirs for uarm-related files
set(UARM_INC ${PROJECT_SOURCE_DIR}/${INC}/uarm)
set(UARM_SRC ${PROJECT_SOURCE_DIR}/${SRC}/uarm)

set(CFLAGS_UARM -mcpu=arm7tdmi)
set(LDFLAGS_UARM -G 0 -nostdlib -T ${UARM_SRC}/elf32ltsarm.h.uarmcore.x)

add_compile_options(${CFLAGS_UARM})


add_library(libuarm OBJECT ${UARM_SRC}/libuarm.s)
add_library(crtso OBJECT ${UARM_SRC}/crtso.s)

target_include_directories(libuarm PRIVATE ${UARM_INC})

add_custom_target(
	kernel.uarm
	DEPENDS crtso libuarm termprint phase0
	COMMAND ${CMAKE_C_LINKER} ${LDFLAGS_UARM}
	$<TARGET_OBJECTS:crtso> $<TARGET_OBJECTS:libuarm>
	$<TARGET_OBJECTS:termprint> $<TARGET_OBJECTS:phase0>
	-o kernel.uarm
)
