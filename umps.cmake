# For each argument passed, apply_elf2umps() interprets it as an uMPS
# executable and applies the umps2-elf2umps step to it, generating a new target
function(apply_elf2umps)
	foreach(target IN LISTS ARGV)
		add_custom_target(
			${target}.core.umps ALL
			COMMAND umps2-elf2umps -k ${PROJECT_BINARY_DIR}/${target}
			BYPRODUCTS ${target}.core.umps ${target}.stab.umps
			DEPENDS ${target}
		)
	endforeach()
endfunction()

# Project include and source dirs for umps-related files
set(UMPS_INC ${BKA_INC}/umps)
set(UMPS_SRC ${BKA_SRC}/umps)

# Flags to be passed to gcc
set(
	CFLAGS_UMPS
	-ffreestanding -ansi -mips1 -mabi=32 -mno-gpopt -G 0 # -mno-abicalls -fno-pic
	-mfp32 -Wall -O0
)
set(LINK_SCRIPT ${UMPS_SRC}/umpscore.ldscript)
# Flags to be passed to ld
set(LDFLAGS_UMPS "-nostdlib -Wl,-G,0,-nostdlib,-T,${LINK_SCRIPT}")

add_compile_options(${CFLAGS_UMPS})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS_UMPS}")
# set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

add_library(libumps ${UMPS_SRC}/libumps.S)
add_library(crtso ${UMPS_SRC}/crtso.S)

# Add test0 executable
add_executable(test0 ${BKA_TEST}/test0.c)
target_link_libraries(test0 crtso libumps io)

# Add test1 executable
add_executable(test1 ${BKA_TEST}/test1.c)
target_link_libraries(test1 crtso libumps io string utils asl)

# Add unit_test executable
add_executable(unit_test ${BKA_TEST}/unit_test.c)
target_link_libraries(unit_test crtso io libumps string utils)

# To each target listed, apply the umps2-elf2umps final step to it
apply_elf2umps(test0 test1 unit_test)
