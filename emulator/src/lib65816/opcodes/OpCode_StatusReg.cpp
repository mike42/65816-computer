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

#define LOG_TAG "Cpu::executeStatusReg"

/**
 * This file contains the implementation for all OpCodes
 * that deal directly with the status register.
 */

void Cpu65816::executeStatusReg(OpCode &opCode) {
    switch (opCode.getCode()) {
        case (0xC2):  // REP #const
        {
            uint8_t value = mSystemBus.readByte(getAddressOfOpCodeData(opCode));
            uint8_t statusByte = mCpuStatus.getRegisterValue();
            mCpuStatus.setRegisterValue(statusByte & ~value);
            addToProgramAddressAndCycles(2, 3);
            break;
        }
        case (0x38):  // SEC
        {
            mCpuStatus.setCarryFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0xF8):  // SED
        {
            mCpuStatus.setDecimalFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0x78):  // SEI
        {
            mCpuStatus.setInterruptDisableFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0x58):  // CLI
        {
            mCpuStatus.clearInterruptDisableFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0xE2):  // SEP
        {
            uint8_t value = mSystemBus.readByte(getAddressOfOpCodeData(opCode));
            if (mCpuStatus.emulationFlag()) {
                // In emulation mode status bits 4 and 5 are not affected
                // 0xCF = 11001111
                value &= 0xCF;
            }
            uint8_t statusReg = mCpuStatus.getRegisterValue();
            statusReg |= value;
            mCpuStatus.setRegisterValue(statusReg);

            addToProgramAddressAndCycles(2, 3);
            break;
        }
        case (0x18):  // CLC
        {
            mCpuStatus.clearCarryFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0xD8):  // CLD
        {
            mCpuStatus.clearDecimalFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0xB8):  // CLV
        {
            mCpuStatus.clearOverflowFlag();
            addToProgramAddressAndCycles(1, 2);
            break;
        }
        case (0xFB):  // XCE
        {
            bool oldCarry = mCpuStatus.carryFlag();
            bool oldEmulation = mCpuStatus.emulationFlag();
            if (oldCarry) mCpuStatus.setEmulationFlag();
            else mCpuStatus.clearEmulationFlag();
            if (oldEmulation) mCpuStatus.setCarryFlag();
            else mCpuStatus.clearCarryFlag();

            mX &= 0xFF;
            mY &= 0xFF;

            if (mCpuStatus.emulationFlag()) {
                mCpuStatus.setAccumulatorWidthFlag();
                mCpuStatus.setIndexWidthFlag();
            } else {
                mCpuStatus.clearAccumulatorWidthFlag();
                mCpuStatus.clearIndexWidthFlag();
            }

            // New stack
            mStack = Stack(&mSystemBus);

            addToProgramAddressAndCycles(1, 2);
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}
