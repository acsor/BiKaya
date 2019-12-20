# Project include, source and library dirs for umips-related files
set(BKA_UMPS_INC ${PROJECT_SOURCE_DIR}/${BKA_INC}/umps)
set(BKA_UMPS_SRC ${PROJECT_SOURCE_DIR}/${BKA_SRC}/umps)

set(CFLAGS_MIPS -mips1 -mabi=32 -mno-gpopt -G 0 -mno-abicalls -fno-pic -mfp32)
set(LDFLAGS_MIPS -G 0 -nostdlib -T ${BKA_UMPS_SRC}/umpscore.ldscript)

add_compile_options(${CFLAGS_MIPS})


add_library(libumps OBJECT ${BKA_UMPS_SRC}/libumps.S)
add_library(crtso OBJECT ${BKA_UMPS_SRC}/crtso.S)

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
