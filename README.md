# BiKaya
BiKaya is an educational-purpose, cross-architecture operating system
compatible with uARM and uM(I)PS2, two micro ISAs derived from ARM and MIPS,
respectively.

## Running
BiKaya (meta) build system is CMake. The list of available executables is

* For uMPS: `test0.core.umps`, `test1.core.umps`, `test2.core.umps `, `unit_test.core.umps`
* For uARM: `test0.uarm`, `test1.uarm`, `test2.uarm`, `unit_test.uarm`
 
 ### Instructor note
 If you are an istructor willing to grade our project, note that the "phase 1.5"
 program can be launched by compiling and running `test2.core.umps`
 and `test2.uarm` executables.

### Build example
As an example, assume you want to compile and then run `test2.core.umps`
and `test2.uarm`. Your terminal steps should then be

1. For the uMPS architecture

	```bash
    mkdir build-umps
    cd build-umps
    cmake -D CMAKE_TOOLCHAIN_FILE=../toolchains/umps.cmake ..
    make test2.core.umps

    # Launch the umps2 emulator
	```

1. For the uARM architecture

	```bash
    mkdir build-uarm
    cd build-uarm
    cmake -D CMAKE_TOOLCHAIN_FILE=../toolchains/uarm.cmake ..
    make test2.uarm

    # Launch the uarm emulator
	```

## Packaging
This option is mostly intended for people working on the project.

### Packaging a source archive
To produce a source archive, use CPack likeso

```bash
# A build for the micro ARM architecture would be fine too
cmake -B build-umps -S . -D CMAKE_TOOLCHAIN_FILE=toolchains/umps.cmake
cd build-umps/
# You can pass other values to -G as well, like ZIP, DEB and more
cpack -G TGZ --config CPackSourceConfig.cmake
# Your source archive should be easily identifiable in ./
```

Packaging features should also be easily accessible from most IDEs.
