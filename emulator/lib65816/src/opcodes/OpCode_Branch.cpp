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

#include <cmath>

#define LOG_TAG "Cpu::executeBranch"

/**
 * This file contains the implementation for all branch OpCodes
 */

int Cpu65816::executeBranchShortOnCondition(bool condition, OpCode &opCode) {
    uint8_t opCycles = 2;
    uint8_t destination =  mSystemBus.readByte(getAddressOfOpCodeData(opCode));
    // This is the address of the next instruction
    uint16_t actualDestination;
    if (condition) {
        // One extra cycle if the branch is taken
        opCycles++;
        uint16_t destination16;
        if (Binary::is8bitValueNegative(destination)) {
            destination16 = 0xFF00 | destination;
        } else {
            destination16 = destination;
        }
        actualDestination = mProgramAddress.getOffset() + 2 + destination16;
        // Emulation mode requires 1 extra cycle on page boundary crossing
        if (Address::offsetsAreOnDifferentPages(mProgramAddress.getOffset(), actualDestination) &&
            mCpuStatus.emulationFlag()) {
            opCycles++;
        }
    } else {
        actualDestination = mProgramAddress.getOffset() + 2;
    }
    Address newProgramAddress(mProgramAddress.getBank(), actualDestination);
    mProgramAddress = newProgramAddress;
    return opCycles;
}

int Cpu65816::executeBranchLongOnCondition(bool condition, OpCode &opCode) {
    if (condition) {
        uint16_t destination = mSystemBus.readTwoBytes(getAddressOfOpCodeData(opCode));
        mProgramAddress.incrementOffsetBy(3 + destination);
    }
    // CPU cycles: 4
    return 4;
}

void Cpu65816::executeBranch(OpCode &opCode) {

    switch(opCode.getCode()) {
        case(0xD0):  // BNE
        {
            addToCycles(executeBranchShortOnCondition(!mCpuStatus.zeroFlag(), opCode));
            break;
        }
        case(0xF0):  // BEQ
        {
            addToCycles(executeBranchShortOnCondition(mCpuStatus.zeroFlag(), opCode));
            break;
        }
        case(0x90):  // BCC
        {
            addToCycles(executeBranchShortOnCondition(!mCpuStatus.carryFlag(), opCode));
            break;
        }
        case(0xB0):  // BCS
        {
            addToCycles(executeBranchShortOnCondition(mCpuStatus.carryFlag(), opCode));
            break;
        }
        case(0x10):  // BPL
        {
            int cycles = executeBranchShortOnCondition(!mCpuStatus.signFlag(), opCode);
            addToCycles(cycles);
            break;
        }
        case(0x30):  // BMI
        {
            addToCycles(executeBranchShortOnCondition(mCpuStatus.signFlag(), opCode));
            break;
        }
        case(0x50):  // BVC
        {
            addToCycles(executeBranchShortOnCondition(!mCpuStatus.overflowFlag(), opCode));
            break;
        }
        case(0x70):  // BVS
        {
            addToCycles(executeBranchShortOnCondition(mCpuStatus.overflowFlag(), opCode));
            break;
        }
        case(0x80):  // BRA
        {
            addToCycles(executeBranchShortOnCondition(true, opCode));
            break;
        }
        case(0x82):  // BRL
        {
            addToCycles(executeBranchLongOnCondition(true, opCode));
            break;
        }
        default: {
            LOG_UNEXPECTED_OPCODE(opCode);
        }
    }
}
