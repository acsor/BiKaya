cmake_minimum_required(VERSION 3.3)
project(BiKaya VERSION 0.1.0 LANGUAGES C ASM)


# All the variables defined in this project start with a BKA_ prefix
set(BKA_INC include)
set(BKA_SRC src)

set(CMAKE_C_STANDARD 90)
set(BKA_CFLAGS_LANG -ffreestanding -ansi -Wall)

# TODO Are we better off using target_include_directories()?
include_directories(${PROJECT_SOURCE_DIR}/${BKA_INC})
include_directories(${PROJECT_BINARY_DIR}/${BKA_INC})

add_compile_options(${BKA_CFLAGS_LANG})
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
# set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")


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

# Project libraries, with alphanumerical ordering
add_library(_bka_test ${BKA_SRC}/bka_test.c)
add_library(asl ${BKA_SRC}/asl.c)
add_library(io ${BKA_SRC}/io.c)
add_library(math ${BKA_SRC}/math.c)
add_library(string ${BKA_SRC}/string.c)
add_library(pcb ${BKA_SRC}/pcb.c)
add_library(phase0 ${BKA_SRC}/phase0.c)
add_library(phase1 ${BKA_SRC}/phase1.c)
add_library(utils ${BKA_SRC}/utils.c)

target_link_libraries(string math)

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
