# 65816 emulator

This is an emulator for the custom 65C816 computer in this repository.

## Before you begin

You will need to assemble the computer's firmware to run the emulator. This is expected to be located at `../rom/rom.bin`.

To build the emulator itself, you will need:

- cmake - tested on 3.22.1
- GCC - tested on 11.2.0.
- Google test and boost libraries, installed on Debian via command below.

```
sudo apt install libgtest-dev libboost1.74-dev libboost-program-options1.74-dev
```

The emulator is only tested on Linux, and only implements hardware features which are used by the computer.

## Quick start

```
cmake .
make && make test
./emulator --rom ../rom/rom.bin
```

## License & Attribution

This emulator is licensed under the GNU General Public License, version 3. See `LICENSE` for details.

It is derived from Francesco Rigoni's [lib65816](https://github.com/FrancescoRigoni/Lib65816), [Lib65816_Sample](https://github.com/FrancescoRigoni/Lib65816_Sample) and [Simple-Logger](https://github.com/FrancescoRigoni/Simple-Logger), with modifications by Michael Billington.
