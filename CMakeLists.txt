cmake_minimum_required(VERSION 3.0)
project(
	BiKaya
	VERSION 0.1.0
	DESCRIPTION "Double-architecture operating system"
	HOMEPAGE_URL https://github.com/newnone/BiKaya
	LANGUAGES C ASM
)


# All the variables defined in this project start with a BKA_ prefix
set(BKA_INC include)
set(BKA_SRC src)

set(CMAKE_C_STANDARD 90)
set(BKA_CFLAGS_LANG -ffreestanding -ansi -Wall)

# TODO Are we better off using target_include_directories()?
include_directories(${PROJECT_SOURCE_DIR}/${BKA_INC})
include_directories(${PROJECT_BINARY_DIR}/${BKA_INC})

add_compile_options(${BKA_CFLAGS_LANG})


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


configure_file(${BKA_INC}/sys.h.in ${PROJECT_BINARY_DIR}/${BKA_INC}/sys.h)

add_library(io OBJECT ${BKA_SRC}/io.c)
add_library(phase0 OBJECT ${BKA_SRC}/phase0.c)


# CPack configuration -- useful for packaging the project in various formats
# (e.g. .tar, .zip, .deb etc.)
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME bikaya)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)
set(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_VERSION_MINOR})
set(CPACK_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

# Exclude unwanted files and directories when generating a source package
list(APPEND CPACK_SOURCE_IGNORE_FILES ".*build.*/")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".git/")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".idea/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "sw[op]$")


include(CPack)
