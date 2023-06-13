# 65816 computer ![example branch parameter](https://github.com/mike42/65816-computer/actions/workflows/build.yml/badge.svg?branch=main)

This is a prototype 65C816-based computer. It features a 1.8432 MHz processor, 1 megabyte of static RAM, a 65C22 I/O port, and an NXP SC16C752 dual UART.

The ROM contains a simple test program.

## Blog series

This is a learning project, and I am blogging about it as I progress.

- [A first look at the 65C816 processor](https://mike42.me/blog/2022-01-a-first-look-at-the-65c816-processor)
- [65C816 computer - initial prototype](https://mike42.me/blog/2022-02-65c816-computer-initial-prototype)
- [Interfacing an NXP UART to an 8-bit computer](https://mike42.me/blog/2022-02-interfacing-an-nxp-uart-to-an-8-bit-computer)
- [Let’s implement preemptive multitasking](https://mike42.me/blog/2022-03-lets-implement-preemptive-multitasking)
- [Building an emulator for my 65C816 computer](https://mike42.me/blog/2022-04-building-an-emulator-for-my-65c816-computer)
- [65C816 computer - second prototype](https://mike42.me/blog/2022-05-65c816-computer-second-prototype)
- [Building a 65C816 test board](https://mike42.me/blog/2022-06-building-a-65c816-test-board)
- [Let’s implement power-on self test (POST)](https://mike42.me/blog/2022-06-lets-implement-power-on-self-test-post)
- [Let’s make a bootloader – Part 1](https://mike42.me/blog/2022-09-lets-make-a-bootloader-part-1)
- [Let’s make a bootloader – Part 2](https://mike42.me/blog/2022-09-lets-make-a-bootloader-part-2)
- [Converting my 65C816 computer project to 3.3 V](https://mike42.me/blog/2023-01-converting-my-65c816-computer-project-to-3-3-v)

## Licenses & acknowledgement

With the exception of the files noted below, this work is © 2022-2023 Michael Billington, and is licensed under a [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/).

### 65816 emulator

The files in the `emulator/` directory are derived from Francesco Rigoni's work, primarily [lib65816](https://github.com/FrancescoRigoni/Lib65816). The emulator application is licensed under the GNU General Public License, version 3. Please see the README and LICENSE file in that directory for details.
