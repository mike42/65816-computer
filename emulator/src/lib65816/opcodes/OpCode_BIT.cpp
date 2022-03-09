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

#include "Interrupt.h"
#include "Cpu65816.h"

#define LOG_TAG "Cpu::executeBIT"

/**
 * This file contains the implementation for all BIT OpCodes
 */

void Cpu65816::execute8BitBIT(OpCode &opCode) {
    const Address addressOfOpCodeData = getAddressOfOpCodeData(opCode);
    uint8_t value = mSystemBus.readByte(addressOfOpCodeData);
    bool isHighestBitSet = value & 0x80;
    bool isNextToHighestBitSet = value & 0x40;

    if (opCode.getAddressingMode() != AddressingMode::Immediate) {
        if (isHighestBitSet) mCpuStatus.setSignFlag();
        else mCpuStatus.clearSignFlag();
        if (isNextToHighestBitSet) mCpuStatus.setOverflowFlag();
        else mCpuStatus.clearOverflowFlag();
    }
    mCpuStatus.updateZeroFlagFrom8BitValue(value & Binary::lower8BitsOf(mA));
}

void Cpu65816::execute16BitBIT(OpCode &opCode) {
    const Address addressOfOpCodeData = getAddressOfOpCodeData(opCode);
    uint16_t value = mSystemBus.readTwoBytes(addressOfOpCodeData);
    bool isHighestBitSet = value & 0x8000;
    bool isNextToHighestBitSet = value & 0x4000;

    if (opCode.getAddressingMode() != AddressingMode::Immediate) {
        if (isHighestBitSet) mCpuStatus.setSignFlag();
        else mCpuStatus.clearSignFlag();
        if (isNextToHighestBitSet) mCpuStatus.setOverflowFlag();
        else mCpuStatus.clearOverflowFlag();
    }
    mCpuStatus.updateZeroFlagFrom16BitValue(value & mA);
}

void Cpu65816::executeBIT(OpCode &opCode) {
    if (accumulatorIs8BitWide()) {
        execute8BitBIT(opCode);
    } else {
        execute16BitBIT(opCode);
        addToCycles(1);
    }

    switch (opCode.getCode()) {
        case (0x89):                 // BIT Immediate
        {
            if (accumulatorIs16BitWide()) {
                addToProgramAddress(1);
            }
            addToProgramAddressAndCycles(2, 2);
            break;
        }
        case (0x2C):                 // BIT Absolute
        {
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case (0x24):                 // BIT Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 3);
            break;
        }
        case (0x3C):                 // BIT Absolute Indexed, X
        {
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(3, 4);
            break;
        }
        case (0x34):                 // BIT Direct Page Indexed, X
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
