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

#define LOG_TAG "Cpu::executeSBC"

/**
 * This file contains the implementation for all SBC OpCodes.
 */

void Cpu65816::execute8BitSBC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint8_t value = mSystemBus.readByte(dataAddress);
    uint8_t accumulator = Binary::lower8BitsOf(mA);
    bool borrow = !mCpuStatus.carryFlag();

    uint16_t result16Bit = accumulator - value - (borrow ? 1 : 0);

    // Is there a borrow from the penultimate bit, redo the diff with 7 bits value and find out.
    accumulator &= 0x7F;
    value &= 0x7F;
    uint8_t partialResult = accumulator - value - (borrow ? 1 : 0);
    // Is bit 8 set?
    bool borrowFromPenultimateBit = partialResult & 0x80;

    // Is there a borrow from the last bit, check bit 8 for that
    bool borrowFromLastBit = result16Bit & 0x0100;

    bool overflow = borrowFromLastBit ^ borrowFromPenultimateBit;
    if (overflow) mCpuStatus.setOverflowFlag();
    else mCpuStatus.clearOverflowFlag();

    if (borrowFromLastBit) mCpuStatus.clearCarryFlag();
    else mCpuStatus.setCarryFlag();

    uint8_t result8Bit = Binary::lower8BitsOf(result16Bit);
    // Update sign and zero flags
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result8Bit);
    // Store the 8 bit result in the accumulator
    Binary::setLower8BitsOf16BitsValue(&mA, result8Bit);
}

void Cpu65816::execute16BitSBC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint16_t value = mSystemBus.readTwoBytes(dataAddress);
    uint16_t accumulator = mA;
    bool borrow = !mCpuStatus.carryFlag();

    uint32_t result32Bit = accumulator - value - (borrow ? 1 : 0);

    // Is there a borrow from the penultimate bit, redo the diff with 15 bits value and find out.
    accumulator &= 0x7FFF;
    value &= 0x7FFF;
    uint16_t partialResult = accumulator - value - (borrow ? 1 : 0);
    // Is bit 15 set?
    bool borrowFromPenultimateBit = partialResult & 0x80;

    // Is there a borrow from the last bit, check bit 16 for that
    bool borrowFromLastBit = result32Bit & 0x0100;

    bool overflow = borrowFromLastBit ^ borrowFromPenultimateBit;
    if (overflow) mCpuStatus.setOverflowFlag();
    else mCpuStatus.clearOverflowFlag();

    if (borrowFromLastBit) mCpuStatus.clearCarryFlag();
    else mCpuStatus.setCarryFlag();

    uint16_t result16Bit = Binary::lower16BitsOf(result32Bit);
    // Update sign and zero flags
    mCpuStatus.updateSignAndZeroFlagFrom16BitValue(result16Bit);
    // Store the 8 bit result in the accumulator
    mA = result16Bit;
}

void Cpu65816::execute8BitBCDSBC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint8_t value = mSystemBus.readByte(dataAddress);
    uint8_t accumulator = Binary::lower8BitsOf(mA);

    uint8_t result = 0;
    bool borrow = Binary::bcdSubtract8Bit(value, accumulator, &result, !mCpuStatus.carryFlag());
    if (borrow) mCpuStatus.clearCarryFlag();
    else mCpuStatus.setCarryFlag();

    Binary::setLower8BitsOf16BitsValue(&mA, result);
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result);
}

void Cpu65816::execute16BitBCDSBC(OpCode &opCode) {
    Address dataAddress = getAddressOfOpCodeData(opCode);
    uint16_t value = mSystemBus.readTwoBytes(dataAddress);
    uint16_t accumulator = mA;

    uint16_t result = 0;
    bool borrow = Binary::bcdSubtract16Bit(value, accumulator, &result, !mCpuStatus.carryFlag());
    if (borrow) mCpuStatus.clearCarryFlag();
    else mCpuStatus.setCarryFlag();

    mA = result;
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(result); // FIXME 16-bit value, possible bug
}

void Cpu65816::executeSBC(OpCode &opCode) {
    if (accumulatorIs8BitWide()) {
        if (mCpuStatus.decimalFlag()) execute8BitBCDSBC(opCode);
        else execute8BitSBC(opCode);
    } else {
        if (mCpuStatus.decimalFlag()) execute16BitBCDSBC(opCode);
        else execute16BitSBC(opCode);
        addToCycles(1);
    }

// All OpCodes take one more cycle on 65C02 in decimal mode
#ifdef EMU_65C02
    if (mCpuStatus.decimalFlag()) {
        addToCycles(1);
    }
#endif

    switch (opCode.getCode()) {
        case (0xE9):                 // SBC Immediate
        {
            if (accumulatorIs16BitWide()) {
                addToProgramAddress(1);
            }
            addToProgramAddress(2);
            addToCycles(2);
            break;
        }
        case (0xED):                 // SBC Absolute
        {
            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case (0xEF):                 // SBC Absolute Long
        {
            addToProgramAddress(4);
            addToCycles(5);
            break;
        }
        case (0xE5):                 // SBC Direct Page
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(3);
            break;
        }
        case (0xF2):                 // SBC Direct Page Indirect
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(5);
            break;
        }
        case (0xE7):                 // SBC Direct Page Indirect Long
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case (0xFD):                 // SBC Absolute Indexed, X
        {
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }

            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case (0xFF):                 // SBC Absolute Long Indexed, X
        {
            addToProgramAddress(4);
            addToCycles(5);
            break;
        }
        case (0xF9):                 // SBC Absolute Indexed Y
        {
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddress(3);
            addToCycles(4);
            break;
        }
        case (0xF5):                 // SBC Direct Page Indexed, X
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddress(2);
            addToCycles(4);
            break;
        }
        case (0xE1):                 // SBC Direct Page Indexed Indirect, X
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case (0xF1):                 // SBC Direct Page Indirect Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            if (opCodeAddressingCrossesPageBoundary(opCode)) {
                addToCycles(1);
            }
            addToProgramAddress(2);
            addToCycles(5);
            break;
        }
        case (0xF7):                 // SBC Direct Page Indirect Long Indexed, Y
        {
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddress(2);
            addToCycles(6);
            break;
        }
        case (0xE3):                 // SBC Stack Relative
        {
            addToProgramAddress(2);
            addToCycles(4);
            break;
        }
        case (0xF3):                 // SBC Stack Relative Indirect Indexed, Y
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
