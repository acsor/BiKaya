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

# Project include, source and library dirs for umips-related files
set(UMPS_INC ${PROJECT_SOURCE_DIR}/${BKA_INC}/umps)
set(UMPS_SRC ${PROJECT_SOURCE_DIR}/${BKA_SRC}/umps)

# Flags to be passed to gcc
set(
	CFLAGS_UMPS
	-ffreestanding -ansi -mips1 -mabi=32 -mno-gpopt -G 0 -mabicalls -fPIC
	-mfp32 -Wall -O0
)
set(LINK_SCRIPT ${UMPS_SRC}/umpscore.ldscript)
# Flags to be passed to ld
set(LDFLAGS_UMPS "-nostdlib -Wl,-G,0,-nostdlib,-T,${LINK_SCRIPT}")

add_compile_options(${CFLAGS_MIPS})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS_UMPS}")
# set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

add_library(libumps ${UMPS_SRC}/libumps.S)
add_library(crtso ${UMPS_SRC}/crtso.S)

# Add kernel0.core.umps executable
add_executable(kernel0 ${BKA_SRC}/phase0.c)
target_link_libraries(kernel0 crtso libumps io)

# Add kernel1.core.umps executable
add_executable(kernel1 ${BKA_SRC}/phase1.c)
target_link_libraries(kernel1 crtso libumps io string utils asl)

# Add bka_test.core.umps executable
add_executable(bka_test ${BKA_SRC}/bka_test.c)
target_link_libraries(bka_test crtso io libumps string utils)

# To each target listed, apply the umps2-elf2umps final step to it
apply_elf2umps(kernel0 kernel1 bka_test)
