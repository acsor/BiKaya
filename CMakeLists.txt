cmake_minimum_required(VERSION 3.0)
project(
	BiKaya
	VERSION 0.1.0
	DESCRIPTION "Double-architecture operating system"
	HOMEPAGE_URL https://github.com/newnone/BiKaya
	LANGUAGES C ASM
)


# The include/ directory is reserved for header files, src/ for
# implementation ones
set(INC include)
set(SRC src)

set(CMAKE_C_STANDARD 90)
set(CFLAGS_LANG -ffreestanding -ansi -Wall)

# TODO Are we better off using target_include_directories()?
include_directories(${PROJECT_SOURCE_DIR}/${INC})
include_directories(${PROJECT_BINARY_DIR}/${INC})

add_compile_options(${CFLAGS_LANG})


# Include targets according to the cross-compiling setup
if (CMAKE_SYSTEM_PROCESSOR STREQUAL mips)
	set(BKA_ARCH_UMPS 1)
	include(umps.cmake)
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
	set(BKA_ARCH_UARM 1)
	include(uarm.cmake)
else()
	message(FATAL_ERROR "You need to specify a toolchain file with either -D
	CMAKE_TOOLCHAIN_FILE=toolchains/umps.cmake or -D
	CMAKE_TOOLCHAIN_FILE=toolchains/uarm.cmake")
endif()


configure_file(${INC}/system.h.in ${PROJECT_BINARY_DIR}/${INC}/system.h)

add_library(termprint OBJECT ${SRC}/termprint.c)
add_library(phase0 OBJECT ${SRC}/phase0.c)
