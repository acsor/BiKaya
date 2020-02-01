# BiKaya
BiKaya is a reduced operating system running on two distinct computer
architectures, micro-ARM and micro-MIPS2. Designed for educational purposes,
this project was commissioned as part of the Operating Systems course from the
University of Bologna, 2019-2020 a.y.

## Installation
BiKaya features several executable targets, each one described in its own
section. In general the CMake build-system is utilized.

### Building phase 0 targets
In order to build the cross-compiled executables, do the following

1. For the micro MIPS architecture

	```bash
	cmake -B build-umps -S . -D CMAKE_TOOLCHAIN_FILE=toolchains/umps.cmake
	cd build-umps
	make kernel.core.umps

	# Launch the umps2 emulator
	```

1. Equivalently, for the micro ARM architecture

	```bash
	cmake -B build-uarm -S . -D CMAKE_TOOLCHAIN_FILE=toolchains/uarm.cmake
	cd build-uarm
	make kernel.uarm

	# Launch the uarm emulator
	```
 
### Building phase 1 targets
Follow the steps exactly as in building targets for the phase0 phase, except
for substituting `kernel.core.umps` with `kernel1.core.umps`, and `kernel.uarm`
with `kernel1.uarm`.

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
