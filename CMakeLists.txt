cmake_minimum_required(VERSION 3.3)
project(BiKaya VERSION 0.3.0 LANGUAGES C ASM)

# All the variables defined in this project start with a BK_ prefix
set(BK_INC ${PROJECT_SOURCE_DIR}/include)
set(BK_BIN_INC ${PROJECT_BINARY_DIR}/include)
set(BK_SRC ${PROJECT_SOURCE_DIR}/src)
set(BK_TEST ${PROJECT_SOURCE_DIR}/test)

set(CMAKE_C_STANDARD 90)
set(BK_CFLAGS_LANG -ffreestanding -ansi -Wall)

# TODO Should we rather use target_include_directories()?
include_directories(${BK_INC})
include_directories(${BK_BIN_INC})

add_compile_options(${BK_CFLAGS_LANG})
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# Include targets according to the cross-compiling setup
if (CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
	add_definitions(-D BK_ARCH_UARM)
	include(cmake/uarm.cmake)
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL mips)
	add_definitions(-D BK_ARCH_UMPS)
	include(cmake/umps.cmake)
else()
	message(FATAL_ERROR "You need to specify a toolchain file with either -D
	CMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake or -D
	CMAKE_TOOLCHAIN_FILE=cmake/mipsel-linux-gnu.cmake")
endif()

# Architecture-independent libraries
add_library(exc ${BK_SRC}/exc.c)
add_library(io ${BK_SRC}/io.c)
add_library(math ${BK_SRC}/math.c)
add_library(pcb ${BK_SRC}/pcb.c)
add_library(sched ${BK_SRC}/sched.c)
add_library(sem ${BK_SRC}/sem.c)
add_library(string ${BK_SRC}/string.c)
add_library(syscall ${BK_SRC}/syscall.c)
add_library(time ${BK_SRC}/time.c)
add_library(utils ${BK_SRC}/utils.c)

# Libraries dependecies
target_link_libraries(io PRIVATE sem)
target_link_libraries(pcb PRIVATE string utils)
target_link_libraries(sched PRIVATE exc pcb time)
target_link_libraries(sem PRIVATE io pcb sched)
target_link_libraries(string PRIVATE math utils)
target_link_libraries(syscall PRIVATE io sched sem time)
target_link_libraries(utils PRIVATE math)

if (CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
	target_link_libraries(math PRIVATE libdiv)
endif()

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
