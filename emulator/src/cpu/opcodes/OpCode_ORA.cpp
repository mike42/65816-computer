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

#define LOG_TAG "Cpu::executeORA"

/**
 * This file contains implementations for all ORA OpCodes.
 */

void Cpu65816::executeORA8Bit(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    uint8_t operand = mSystemBus.readByte(opCodeDataAddress);
    uint8_t result = Binary::lower8BitsOf(mA) | operand;
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result);
    Binary::setLower8BitsOf16BitsValue(&mA, result);
}

void Cpu65816::executeORA16Bit(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    uint16_t operand = mSystemBus.readTwoBytes(opCodeDataAddress);
    uint16_t result = mA | operand;
    mCpuStatus.updateSignAndZeroFlagFrom16BitValue(result);
    mA = result;
}

void Cpu65816::executeORA(OpCode &opCode) {
    if (accumulatorIs8BitWide()) {
        executeORA8Bit(opCode);
    } else {
        executeORA16Bit(opCode);
        addToCycles(1);
    }

    switch (opCode.getCode()) {
        case (0x09):                // ORA Immediate
        {
            if (accumulatorIs16BitWide()) {
                addToProgramAddress(1);
            }
            addToProgramAddressAndCycles(2, 2);
            break;
        }
        case (0x0D):                // ORA Absolute
        {
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case (0x0F):                // ORA Absolute Long
        {
            addToProgramAddressAndCycles(4, 5);
            break;
        }
        case (0x05):                 // ORA Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 3);
            break;
        }
        case (0x12):                 // ORA Direct Page Indirect
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 5);
            break;
        }
        case (0x07):                 // ORA Direct Page Indirect Long
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 6);
            break;
        }
        case (0x1D):                 // ORA Absolute Indexed, X
        {
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case (0x1F):                 // ORA Absolute Long Indexed, X
        {
            addToProgramAddressAndCycles(4, 5);
            break;
        }
        case (0x19):                 // ORA Absolute Indexed, Y
        {
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case (0x15):                 // ORA Direct Page Indexed, X
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 4);
            break;
        }
        case (0x01):                // ORA Direct Page Indexed Indirect, X
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 6);
            break;
        }
        case (0x11):                 // ORA Direct Page Indirect Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 5);
            break;
        }
        case (0x17):                 // ORA Direct Page Indirect Long Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 6);
            break;
        }
        case (0x03):                // ORA Stack Relative
        {
            addToProgramAddressAndCycles(2, 4);
            break;
        }
        case (0x13):                 // ORA Stack Relative Indirect Indexed, Y
        {
            addToProgramAddressAndCycles(2, 7);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}
