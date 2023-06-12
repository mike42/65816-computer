#!/bin/bash
set -exu -o pipefail

# Build, test the emulator
(cd emulator && \
  cmake -B build/release && \
  make -j8 -C build/release all test)

# Build fuzzers, do short run just to confirm they start
(cd emulator &&
  CC=clang CXX=clang++ cmake -DFUZZ=1 -B build/fuzz && \
  make -j8 -C build/fuzz && \
  ./build/fuzz/fuzz_debug_symbols_parser -seed=1 -runs=100)

# Build the ROM (outputs multiple files - rom/rom_emulator.bin required for emulator)
make -C rom all

# Build software
make -C software disk.img hexdump.img snake.img

