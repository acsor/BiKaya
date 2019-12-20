# Project include, source and library dirs for umips-related files
# TODO Add BKA prefix to project-local variables
set(UMPS_INC ${PROJECT_SOURCE_DIR}/${INC}/umps)
set(UMPS_SRC ${PROJECT_SOURCE_DIR}/${SRC}/umps)

set(CFLAGS_MIPS -mips1 -mabi=32 -mno-gpopt -G 0 -mno-abicalls -fno-pic -mfp32)
set(LDFLAGS_MIPS -G 0 -nostdlib -T ${UMPS_SRC}/umpscore.ldscript)

add_compile_options(${CFLAGS_MIPS})


add_library(libumps OBJECT ${UMPS_SRC}/libumps.S)
add_library(crtso OBJECT ${UMPS_SRC}/crtso.S)

add_custom_command(
	OUTPUT kernel
	DEPENDS crtso libumps termprint phase0
	COMMAND ${CMAKE_C_LINKER} ${LDFLAGS_MIPS}
	$<TARGET_OBJECTS:crtso> $<TARGET_OBJECTS:libumps>
	$<TARGET_OBJECTS:termprint> $<TARGET_OBJECTS:phase0>
	-o kernel
)
# Run the `umps2-elf2umps -k kernel' command after building `kernel'
add_custom_target(
	kernel.core.umps
	COMMAND umps2-elf2umps -k ${PROJECT_BINARY_DIR}/kernel
	DEPENDS kernel
)
