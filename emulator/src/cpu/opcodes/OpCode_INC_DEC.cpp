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

#define LOG_TAG "Cpu::executeINCDEC"

/**
 * This file contains implementations for all Increment and Decrement OpCodes.
 */

void Cpu65816::execute8BitDecInMemory(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    uint8_t value = mSystemBus.readByte(opCodeDataAddress);
    value--;
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(value);
    mSystemBus.storeByte(opCodeDataAddress, value);
}

void Cpu65816::execute16BitDecInMemory(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    uint16_t value = mSystemBus.readTwoBytes(opCodeDataAddress);
    value--;
    mCpuStatus.updateSignAndZeroFlagFrom16BitValue(value);
    mSystemBus.storeTwoBytes(opCodeDataAddress, value);
}

void Cpu65816::execute8BitIncInMemory(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    uint8_t value = mSystemBus.readByte(opCodeDataAddress);
    value++;
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(value);
    mSystemBus.storeByte(opCodeDataAddress, value);
}

void Cpu65816::execute16BitIncInMemory(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);
    uint16_t value = mSystemBus.readTwoBytes(opCodeDataAddress);
    value++;
    mCpuStatus.updateSignAndZeroFlagFrom16BitValue(value);
    mSystemBus.storeTwoBytes(opCodeDataAddress, value);
}

void Cpu65816::executeINCDEC(OpCode &opCode) {
    switch (opCode.getCode()) {
        case (0x1A):  // INC Accumulator
        {
            if (accumulatorIs8BitWide()) {
                uint8_t lowerA = Binary::lower8BitsOf(mA);
                lowerA++;
                Binary::setLower8BitsOf16BitsValue(&mA, lowerA);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerA);
            } else {
                mA++;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0xEE): // INC Absolute
        {
            if (accumulatorIs8BitWide()) {
                execute8BitIncInMemory(opCode);
            } else {
                execute16BitIncInMemory(opCode);
                addToCycles(2);
            }
            addToProgramAddressAndCycles(3, 6);
            break;
        }
        case (0xE6): // INC Direct Page
        {
            if (accumulatorIs8BitWide()) {
                execute8BitIncInMemory(opCode);
            } else {
                execute16BitIncInMemory(opCode);
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 5);
            break;
        }
        case (0xFE): // INC Absolute Indexed, X
        {
            if (accumulatorIs8BitWide()) {
                execute8BitIncInMemory(opCode);
            } else {
                execute16BitIncInMemory(opCode);
                addToCycles(2);
            }
#ifdef EMU_65C02
            if (!opCodeAddressingCrossesPageBoundary(opCode)) {
                subtractFromCycles(1);
            }
#endif
            addToProgramAddressAndCycles(3, 7);
        }
            break;
        case (0xF6): // INC Direct Page Indexed, X
        {
            if (accumulatorIs8BitWide()) {
                execute8BitIncInMemory(opCode);
            } else {
                execute16BitIncInMemory(opCode);
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 6);
            break;
        }
        case (0x3A):  // DEC Accumulator
        {
            if (accumulatorIs8BitWide()) {
                uint8_t lowerA = Binary::lower8BitsOf(mA);
                lowerA--;
                Binary::setLower8BitsOf16BitsValue(&mA, lowerA);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerA);
            } else {
                mA--;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mA);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0xCE): // DEC Absolute
        {
            if (accumulatorIs8BitWide()) {
                execute8BitDecInMemory(opCode);
            } else {
                execute16BitDecInMemory(opCode);
                addToCycles(2);
            }
            addToProgramAddressAndCycles(3, 6);
            break;
        }
        case (0xC6): // DEC Direct Page
        {
            if (accumulatorIs8BitWide()) {
                execute8BitDecInMemory(opCode);
            } else {
                execute16BitDecInMemory(opCode);
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 5);
            break;
        }
        case (0xDE): // DEC Absolute Indexed, X
        {
            if (accumulatorIs8BitWide()) {
                execute8BitDecInMemory(opCode);
            } else {
                execute16BitDecInMemory(opCode);
                addToCycles(2);
            }
#ifdef EMU_65C02
            if (!opCodeAddressingCrossesPageBoundary(opCode)) {
                subtractFromCycles(1);
            }
#endif
            addToProgramAddressAndCycles(3, 7);
            break;
        }
        case (0xD6): // DEC Direct Page Indexed, X
        {
            if (accumulatorIs8BitWide()) {
                execute8BitDecInMemory(opCode);
            } else {
                execute16BitDecInMemory(opCode);
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }
            addToProgramAddressAndCycles(2, 6);
            break;
        }
        case (0xC8):  // INY
        {
            if (indexIs8BitWide()) {
                uint8_t lowerY = Binary::lower8BitsOf(mY);
                lowerY++;
                Binary::setLower8BitsOf16BitsValue(&mY, lowerY);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerY);
            } else {
                mY++;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mY);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0xE8):  // INX
        {
            if (indexIs8BitWide()) {
                uint8_t lowerX = Binary::lower8BitsOf(mX);
                lowerX++;
                Binary::setLower8BitsOf16BitsValue(&mX, lowerX);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerX);
            } else {
                mX++;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mX);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0x88):  // DEY
        {
            if (indexIs8BitWide()) {
                uint8_t lowerY = Binary::lower8BitsOf(mY);
                lowerY--;
                Binary::setLower8BitsOf16BitsValue(&mY, lowerY);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerY);
            } else {
                mY--;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mY);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0xCA):  // DEX
        {
            if (indexIs8BitWide()) {
                uint8_t lowerX = Binary::lower8BitsOf(mX);
                lowerX--;
                Binary::setLower8BitsOf16BitsValue(&mX, lowerX);
                mCpuStatus.updateSignAndZeroFlagFrom8BitValue(lowerX);
            } else {
                mX--;
                mCpuStatus.updateSignAndZeroFlagFrom16BitValue(mX);
            }
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}
