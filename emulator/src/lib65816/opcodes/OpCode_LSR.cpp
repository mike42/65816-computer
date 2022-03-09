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

#define LOG_TAG "Cpu::executeLSR"

#define DO_LSR_8_BIT(value) {                                   \
    bool newCarry = value & 0x01;                               \
    value = value >> 1;                                         \
    if (newCarry) mCpuStatus.setCarryFlag();                    \
    else mCpuStatus.clearCarryFlag();                           \
    mCpuStatus.updateSignAndZeroFlagFrom8BitValue(value);       \
}

#define DO_LSR_16_BIT(value) {                                  \
    bool newCarry = value & 0x0001;                             \
    value = value >> 1;                                         \
    if (newCarry) mCpuStatus.setCarryFlag();                    \
    else mCpuStatus.clearCarryFlag();                           \
    mCpuStatus.updateSignAndZeroFlagFrom16BitValue(value);      \
}

/**
 * This file contains implementations for all LSR OpCodes.
 */

void Cpu65816::executeMemoryLSR(OpCode &opCode) {
    Address opCodeDataAddress = getAddressOfOpCodeData(opCode);

    if (accumulatorIs8BitWide()) {
        uint8_t value = mSystemBus.readByte(opCodeDataAddress);
        DO_LSR_8_BIT(value);
        mSystemBus.storeByte(opCodeDataAddress, value);
    } else {
        uint16_t value = mSystemBus.readTwoBytes(opCodeDataAddress);
        DO_LSR_16_BIT(value);
        mSystemBus.storeTwoBytes(opCodeDataAddress, value);
    }
}

void Cpu65816::executeAccumulatorLSR(OpCode &opCode) {
    if (accumulatorIs8BitWide()) {
        uint8_t value = Binary::lower8BitsOf(mA);
        DO_LSR_8_BIT(value);
        Binary::setLower8BitsOf16BitsValue(&mA, value);
    } else {
        DO_LSR_16_BIT(mA);
    }
}

void Cpu65816::executeLSR(OpCode &opCode) {
    switch (opCode.getCode()) {
        case (0x4A):                // LSR Accumulator
        {
            executeAccumulatorLSR(opCode);
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0x4E):                // LSR Absolute
        {
            executeMemoryLSR(opCode);
            if (accumulatorIs16BitWide()) {
                addToCycles(2);
            }
            addToProgramAddressAndCycles(3, 6);
            break;
        }
        case (0x46):                // LSR Direct Page
        {
            executeMemoryLSR(opCode);
            if (accumulatorIs16BitWide()) {
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddressAndCycles(2, 5);
            break;
        }
        case (0x5E):                // LSR Absolute Indexed, X
        {
            executeMemoryLSR(opCode);
            if (accumulatorIs16BitWide()) {
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
        case (0x56):                // LSR Direct Page Indexed, X
        {
            executeMemoryLSR(opCode);
            if (accumulatorIs16BitWide()) {
                addToCycles(2);
            }
            if (Binary::lower8BitsOf(mD) != 0) {
                addToCycles(1);
            }

            addToProgramAddressAndCycles(2, 6);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}
