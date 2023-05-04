# 65816 emulator

This is an emulator for the custom 65C816 computer in this repository.

## Before you begin

You will need to assemble the computer's firmware to run the emulator. This is expected to be located at `../rom/rom.bin`.

To build the emulator itself, you will need:

- cmake - tested on 3.22.1
- GCC - tested on 11.2.0.
- Google test and boost libraries, installed on Debian via command below.

```
sudo apt install libgtest-dev libboost1.74-dev libboost-program-options1.74-dev libboost-filesystem1.74-dev
```

The emulator is only tested on Linux, and only implements hardware features which are used by the computer.

## Quick start

Compile and test a release build:

```
cmake -B build/release
make -j8 -C build/release all test
```

Run the emulator:

```
./build/release/emulator --rom ../rom/rom.bin
```

This will create a unix socket called 'serial1' in the current directory. In a separate terminal window, connect to this socket with minicom to interact with the emulated system.

```
minicom -D unix#serial
```

## Test mode

The emulator also functions as a unit test runner for the 65816 assembly projects in this repository. It uses debug information (a list of labels in VICE format) to locate and execute a `test_setup` routine, followed by every routine beginning with the name `test_`. If a routine leaves 0 in the accumulator after returning, it is recorded as a test pass. Any other value is recorded as a test failure.

An example project is located in the `test/resources/testing_example/` directory to show how this feature works. Execute the tests with the following command:

```
./build/release/emulator --test test/resources/testing_example/test.bin
```

## Fuzz tests

Fuzz tests are not built by default, since they depend on different build flags, as well as the use of clang.

Build them with the following commands:

```
CC=clang CXX=clang++ cmake -DFUZZ=1 -B build/fuzz
make -j8 -C build/fuzz
```

Next, run your fuzzer of choice from the `build/fuzz` directory.

## License & Attribution

This emulator is licensed under the GNU General Public License, version 3. See `LICENSE` for details.

It is derived from Francesco Rigoni's [lib65816](https://github.com/FrancescoRigoni/Lib65816), [Lib65816_Sample](https://github.com/FrancescoRigoni/Lib65816_Sample) and [Simple-Logger](https://github.com/FrancescoRigoni/Simple-Logger), with modifications by Michael Billington.
