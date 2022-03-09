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

#include "Cpu65816.hpp"

#define LOG_TAG "Cpu::executeSTA"

/**
 * This file contains the implementation for all STA OpCodes.
 */

void Cpu65816::executeSTA(OpCode &opCode) {

    Address dataAddress = getAddressOfOpCodeData(opCode);
    if (accumulatorIs8BitWide()) {
        mSystemBus.storeByte(dataAddress, Binary::lower8BitsOf(mA));
    } else {
        mSystemBus.storeTwoBytes(dataAddress, mA);
        addToCycles(1);
    }

    switch (opCode.getCode()) {
        case(0x8D):  // STA Absolute
        {
            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case(0x8F):  // STA Absolute Long
        {
            addToProgramAddress(4);
            addToCycles(5);
            break;
        }
        case(0x85):  // STA Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(3);
            break;
        }
        case(0x92):  // STA Direct Page Indirect
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(5);
            break;
        }
        case(0x87):  // STA Direct Page Indirect Long
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case(0x9D):  // STA Absolute Indexed, X
        {
            addToProgramAddress(3);
            addToCycles(5);
            break;
        }
        case(0x9F):  // STA Absolute Long Indexed, X
        {
            addToProgramAddress(4);
            addToCycles(5);
            break;
        }
        case(0x99):  // STA Absolute Indexed, Y
        {
            addToProgramAddress(3);
            addToCycles(5);
            break;
        }
        case(0x95):  // STA Direct Page Indexed, X
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(4);
            break;
        }
        case(0x81):  // STA Direct Page Indexed Indirect, X
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case(0x91):  // STA Direct Page Indirect Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case(0x97):  // STA Direct Page Indirect Long Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case(0x83):  // STA Stack Relative
        {
            addToProgramAddress(2);
            addToCycles(4);
            break;
        }
        case(0x93):  // STA Stack Relative Indirect Indexed, Y
        {
            addToProgramAddress(2);
            addToCycles(7);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}
