# BiKaya
BiKaya is an educational-purpose, cross-architecture operating system
compatible with [uARM][uarm] and [uM(I)PS2][umps], two micro ISAs derived from
[ARM][arm] and [MIPS][mips], respectively.

[uarm]: contrib/uARM-Informal-Specifications.pdf
[umps]: contrib/uMPS2-Principles-of-Operation.pdf
[arm]: https://en.wikipedia.org/wiki/ARM_architecture
[mips]: https://en.wikipedia.org/wiki/MIPS_architecture

## Running
BiKaya (meta) build system is CMake. The list of executables available to run is

* For uARM: `test0.uarm`, `test1.uarm`, `test2.uarm`, `test3.uarm`,
`unit_test.uarm`
* For uMPS: `test0.core.umps`, `test1.core.umps`, `test2.core.umps `,
`test3.core.umps`, `unit_test.core.umps`

### Instructor's note (italian paragraph)
Il programma di test per la fase 2 è `test3.uarm` per uARM e `test3.core.umps`
per uMPS. Rifarsi alla sezione seguente per le istruzioni sul come compilare.

### Build instructions
As an example, assume you want to compile and then run `test2.uarm` and
`test2.core.umps`. Your terminal steps should then be

1. For the uARM architecture

	```bash
    mkdir build-uarm
    cd build-uarm
    cmake -D CMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.cmake ..
    make test2.uarm

    # Launch the uarm emulator
    ```

1. For the uMPS architecture

	```bash
    mkdir build-umps
    cd build-umps
    cmake -D CMAKE_TOOLCHAIN_FILE=../cmake/mipsel-linux-gnu.cmake ..
    make test2.core.umps

    # Launch the umps2 emulator
	```

#### Architecture emulators
Since the uARM and uMPS architectures are intended to be emulated, it is
unlikely for you to be able to run the compiled code in the host machine. In
order to execute the compiled binaries, you might check out the availble uARM
and uMPS emulators. See

* [mellotanica/uARM](https://github.com/mellotanica/uARM)
* [tjonjic/umps](https://github.com/tjonjic/umps)

Installing the required dependecies might require some figuring out on your
part. Good luck! (If you can read Italian, see [phase0_2020.pdf][phase0].)

[phase0]: contrib/phase0_2020.pdf
 
#### Build insructions: alternative toolchain(s)
The [CMAKE_TOOLCHAIN_FILE][ctf] command-line variabile passed to CMake instructs
it on where to find the cross-compiling toolchain. Since we are building for
architectures other than the host one, we cannot use the ordinary toolchain
(e.g. `gcc`).

It might be that your operating system (e.g. Linux distribution) doesn't have
an easy access to the toolchains our project looks for, or that you have any
other compelling reason to switch from the ones you already own. If that is the
case, you may obtain the required components to compile our project by
downloading and installing them with [crosstool-ng][ct-ng]. That's where the
`CMAKE_TOOLCHAIN_FILE` variable comes in handy: if the toolchain is installed
locally, you can provide an alternative file location to it, like
`mipsel-linux-gnu-local.cmake`, filled in like so

```cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR mips)
set(TOOLCHAIN_ROOT /home/user/.../mipsel-unknown-linux-gnu)     # 1
set(TOOLCHAIN_PREFIX mipsel-linux-gnu)                          # 2

set(CMAKE_C_COMPILER ${TOOLCHAIN_ROOT}/bin/${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_ROOT}/bin/${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_C_LINKER ${TOOLCHAIN_ROOT}/bin/${TOOLCHAIN_PREFIX}-ld)
```

Ideally, you should only change lines `1` and `2` to match your local
configuration. The particular instructions, and even the name itself of the
file, are entirely arbitrary suggestions. See more in the [official CMake
documentation][cdoc].

[ctf]: https://cmake.org/cmake/help/latest/variable/CMAKE_TOOLCHAIN_FILE.html
[ct-ng]: https://crosstool-ng.github.io/
[cdoc]: https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html

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
