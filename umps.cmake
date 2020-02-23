# Project include, source and library dirs for umips-related files
set(BKA_UMPS_INC ${PROJECT_SOURCE_DIR}/${BKA_INC}/umps)
set(BKA_UMPS_SRC ${PROJECT_SOURCE_DIR}/${BKA_SRC}/umps)

set(CFLAGS_MIPS -mips1 -mabi=32 -mno-gpopt -G 0 -mno-abicalls -fno-pic -mfp32)
set(LDFLAGS_MIPS -G 0 -nostdlib -T ${BKA_UMPS_SRC}/umpscore.ldscript)

add_compile_options(${CFLAGS_MIPS})


add_library(libumps OBJECT ${BKA_UMPS_SRC}/libumps.S)
add_library(crtso OBJECT ${BKA_UMPS_SRC}/crtso.S)

add_custom_command(
	OUTPUT kernel0
	DEPENDS crtso libumps io phase0
	COMMAND ${CMAKE_C_LINKER} ${LDFLAGS_MIPS}
	$<TARGET_OBJECTS:crtso> $<TARGET_OBJECTS:libumps>
	$<TARGET_OBJECTS:io> $<TARGET_OBJECTS:phase0>
	-o kernel0
)
# Run the `umps2-elf2umps -k kernel0' command after building `kernel0'
add_custom_target(
	kernel0.core.umps
	COMMAND umps2-elf2umps -k ${PROJECT_BINARY_DIR}/kernel0
	DEPENDS kernel0
)

# TODO Set the test build instructions in a more idiomatic way
add_custom_command(
	OUTPUT kernel1
	DEPENDS crtso libumps io math string utils pcb asl phase1
	COMMAND ${CMAKE_C_LINKER} ${LDFLAGS_MIPS}
	$<TARGET_OBJECTS:crtso> $<TARGET_OBJECTS:libumps>
	$<TARGET_OBJECTS:io> $<TARGET_OBJECTS:math> $<TARGET_OBJECTS:string>
	$<TARGET_OBJECTS:utils>
	$<TARGET_OBJECTS:phase1> $<TARGET_OBJECTS:pcb> $<TARGET_OBJECTS:asl>
	-o kernel1
)
add_custom_target(
	kernel1.core.umps
	COMMAND umps2-elf2umps -k ${PROJECT_BINARY_DIR}/kernel1
	DEPENDS kernel1
)

add_custom_command(
	OUTPUT bka_test
	DEPENDS crtso libumps io math string utils _bka_test
	COMMAND ${CMAKE_C_LINKER} ${LDFLAGS_MIPS}
	$<TARGET_OBJECTS:crtso> $<TARGET_OBJECTS:libumps>
	$<TARGET_OBJECTS:io> $<TARGET_OBJECTS:math> $<TARGET_OBJECTS:string>
	$<TARGET_OBJECTS:utils> $<TARGET_OBJECTS:_bka_test>
	-o bka_test
)
add_custom_target(
	bka_test.core.umps
	COMMAND umps2-elf2umps -k ${PROJECT_BINARY_DIR}/bka_test
	DEPENDS bka_test
)
