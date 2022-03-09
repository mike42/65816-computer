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

#include <cmath>

#define LOG_TAG "Cpu::executeLDY"

/**
 * This file contains implementations for all LDY OpCodes.
 */

void Cpu65816::executeLDY8Bit(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    uint8_t value = mSystemBus.readByte(opCodeDataAddress);
    Binary::setLower8BitsOf16BitsValue(&mY, value);
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(value);
}

void Cpu65816::executeLDY16Bit(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    mY = mSystemBus.readTwoBytes(opCodeDataAddress);
    mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mY);
}

void Cpu65816::executeLDY(OpCode &opCode) {
    if (indexIs16BitWide()) {
        executeLDY16Bit(opCode);
        addToCycles(1);
    } else {
        executeLDY8Bit(opCode);
    }

    switch (opCode.getCode()) {
        case (0xA0):                // LDY Immediate
        {
            if (indexIs16BitWide()) {
                addToProgramAddress(1);
            }
            addToProgramAddressAndCycles(2, 2);
            break;
        }
        case (0xAC):                // LDY Absolute
        {
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case (0xA4):                // LDY Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 3);
            break;
        }
        case (0xBC):                // LDY Absolute Indexed, X
        {
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case (0xB4):                // LDY Direct Page Indexed, X
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 4);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}
