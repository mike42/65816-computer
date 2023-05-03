/*
 * This file is part of the 65816 Emulator Library.
 * Copyright (c) 2018 Francesco Rigoni.
 *
 * https://github.com/FrancescoRigoni/Lib65816
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EMULATOR_STACK_H
#define EMULATOR_STACK_H

#include <cstdint>
#include "SystemBus.h"

#define STACK_POINTER_DEFAULT 0x1FF

class Stack {
public:

    explicit Stack(SystemBus *);
    Stack(SystemBus *, uint16_t);

    void push8Bit(uint8_t);
    void push16Bit(uint16_t);

    uint8_t pull8Bit();
    uint16_t pull16Bit();

    uint16_t getStackPointer();

private:
    SystemBus *mSystemBus;
    Address mStackAddress;
};

#endif // EMULATOR_STACK_H
