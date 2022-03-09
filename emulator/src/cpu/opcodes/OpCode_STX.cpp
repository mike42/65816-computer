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

#include "Cpu65816.h"

#define LOG_TAG "Cpu::executeSTX"

/**
 * This file contains the implementation for all STX OpCodes.
 */

void Cpu65816::executeSTX(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    if (accumulatorIs8BitWide()) {
        mSystemBus.storeByte(dataAddress, Binary::lower8BitsOf(mX));
    } else {
        mSystemBus.storeTwoBytes(dataAddress, mX);
        addToCycles(1);
    }

    switch (opCode.getCode()) {
        case (0x8E):  // STX Absolute
        {
            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case (0x86):  // STX Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(3);
            break;
        }
        case (0x96):  // STX Direct Page Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(4);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}
