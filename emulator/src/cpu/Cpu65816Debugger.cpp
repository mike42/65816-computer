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

#include "Cpu65816Debugger.h"
#include "Cpu65816.h"

#define LOG_TAG "Cpu65816Debugger"

Cpu65816Debugger::Cpu65816Debugger(Cpu65816 &cpu, DebugSymbols &symbols, bool &trace): mCpu(cpu), mSymbols(symbols), mTrace(trace) {
    cpu.setRESPin(false);

    Log::dbg(LOG_TAG).str("Cpu is ready to run").show();
}

void Cpu65816Debugger::step() {
    if (mBreakpointHit) return;

    mOnBeforeStepHandler();
    const uint8_t instruction = mCpu.mSystemBus.readByte(mCpu.mProgramAddress);
    OpCode opCode = mCpu.OP_CODE_TABLE[instruction];
    if(opCode.getCode() == 0xDB) {
        // STP instruction handling.
        mStpHandler();
    }

    logOpCode(opCode);

    mCpu.executeNextInstruction();

    mOnAfterStepHandler();

    if (mCpu.mProgramAddress.getBank() == mBreakPointAddress.getBank() &&
        mCpu.mProgramAddress.getOffset() == mBreakPointAddress.getOffset()) {
        mBreakpointHit = true;
        Log::dbg(LOG_TAG).str("BREAKPOINT").sp()
                .hex(mBreakPointAddress.getBank(), 2).hex(mBreakPointAddress.getOffset(), 4).show();
        mOnBreakPointHandler();
    }
}

void Cpu65816Debugger::doBeforeStep(const std::function<void()> handler) {
    mOnBeforeStepHandler = handler;
}

void Cpu65816Debugger::doAfterStep(const std::function<void()> handler) {
    mOnAfterStepHandler = handler;
}

void Cpu65816Debugger::onBreakPoint(const std::function<void()> handler) {
    mOnBreakPointHandler = handler;
}

void Cpu65816Debugger::setBreakPoint(const Address &address) {
    mBreakPointAddress = address;
}

void Cpu65816Debugger::onStp(const std::function<void()> handler) {
    mStpHandler = handler;
}

void Cpu65816Debugger::logStatusRegister() const {
    Log::trc(LOG_TAG).str("P (Status): ").hex(mCpu.mCpuStatus.getRegisterValue(), 2).show();
}

void Cpu65816Debugger::dumpCpu() const {
    Log::trc(LOG_TAG).str("====== CPU status start ======").show();
    Log::trc(LOG_TAG).str("A: ").hex(mCpu.mA, 4).sp().str("X: ").hex(mCpu.mX, 4).sp().str("Y: ").hex(mCpu.mY, 4).show();
    Log::trc(LOG_TAG).str("PB: ").hex(mCpu.mProgramAddress.getBank(), 2).sp().str("PC: ").hex(
            mCpu.mProgramAddress.getOffset(), 4).show();
    Log::trc(LOG_TAG).str("DB: ").hex(mCpu.mDB, 2).sp().str("D: ").hex(mCpu.mD, 4).show();
    Log::trc(LOG_TAG).str("S (Stack pointer): ").hex(mCpu.mStack.getStackPointer(), 4).show();
    logStatusRegister();
    Log::trc(LOG_TAG).str("====== CPU status end ======").show();
}

std::string hex(uint32_t val, int width) {
    std::ostringstream out;
    out << '$' << std::setw(width) << std::setfill('0') << std::hex << val;
    return out.str();
}

void Cpu65816Debugger::logOpCode(OpCode &opCode) const {
    if(!mTrace) {
        return;
    }

    std::ostringstream stream;
    int symbolSpace = 30;
    Address onePlusOpCodeAddress = mCpu.mProgramAddress.newWithOffset(1);

    // Report program counter location
    uint32_t absoluteLongProgramCounter = mCpu.mProgramAddress.getAbsolute();
    std::string reportedName;
    if(mSymbols.labelsReverse.contains(absoluteLongProgramCounter)) {
        reportedName = mSymbols.labelsReverse[absoluteLongProgramCounter];
    }
    stream << hex(absoluteLongProgramCounter, 6) << ": " << std::left << std::setw(symbolSpace) << reportedName;
    stream << hex(opCode.getCode(), 2) << ' ' << opCode.getName() << ' ';

    switch (opCode.getAddressingMode()) {
        case AddressingMode::Interrupt:
        case AddressingMode::Accumulator:
        case AddressingMode::Implied:
            break;
        case AddressingMode::Immediate:
            // This refers to accumulator size to estimate the kind of value to print.
            // Instructions using index registers might print the wrong value.
            if (mCpu.accumulatorIs8BitWide()) {
                stream << '#' << hex(mCpu.mSystemBus.readByte(mCpu.getAddressOfOpCodeData(opCode)), 2);
            } else {
                stream << '#' << hex(mCpu.mSystemBus.readTwoBytes(mCpu.getAddressOfOpCodeData(opCode)), 4);
            }
            break;
        case AddressingMode::Absolute:
            stream << hex(mCpu.getAddressOfOpCodeData(opCode).getOffset(), 4) << ' ';
            stream << "                    [Absolute]";
            break;
        case AddressingMode::AbsoluteLong: {
            Address opCodeDataAddress = mCpu.getAddressOfOpCodeData(opCode);
            stream << hex(opCodeDataAddress.getAbsolute(), 6) << ' ';
            stream << "                  [Absolute Long]";
        }
            break;
        case AddressingMode::AbsoluteIndirect:
            break;
        case AddressingMode::AbsoluteIndirectLong:
            break;
        case AddressingMode::AbsoluteIndexedIndirectWithX:
            stream << "(" << hex(mCpu.mSystemBus.readTwoBytes(onePlusOpCodeAddress), 4) << ", X)";
            stream << "                [Absolute Indexed Indirect, X]";
            break;
        case AddressingMode::AbsoluteIndexedWithX:
            stream << hex(mCpu.mSystemBus.readTwoBytes(onePlusOpCodeAddress), 4) << ", X" << " ";
            stream << "                 [Absolute Indexed, X]";
            break;
        case AddressingMode::AbsoluteLongIndexedWithX: {
            Address opCodeDataAddress = mCpu.getAddressOfOpCodeData(opCode);
            Address effectiveAddress = mCpu.mSystemBus.readAddressAt(opCodeDataAddress);
            stream << hex(effectiveAddress.getBank(), 2) << ":" << hex(effectiveAddress.getOffset(), 4) << ", X" << " ";
            stream << "             [Absolute Long Indexed, X]";
        }
            break;
        case AddressingMode::AbsoluteIndexedWithY:
            stream << hex(mCpu.mSystemBus.readTwoBytes(mCpu.getAddressOfOpCodeData(opCode)), 4)  << ", Y" << " ";
            stream << "                    [Absolute Indexed, Y]";
            break;
        case AddressingMode::DirectPage:
            stream << hex(mCpu.mSystemBus.readByte(onePlusOpCodeAddress), 2) << " ";
            stream << "                      [Direct Page]";
            break;
        case AddressingMode::DirectPageIndexedWithX:
            stream << hex(mCpu.mSystemBus.readByte(onePlusOpCodeAddress), 2)  << ", X" << " ";
            stream << "                   [Direct Page Indexed, X]";
            break;
        case AddressingMode::DirectPageIndexedWithY:
            stream << hex(mCpu.mSystemBus.readByte(onePlusOpCodeAddress), 2)  << ", Y" << " ";
            stream << "                    [Direct Page Indexed, Y]";
            break;
        case AddressingMode::DirectPageIndirect:
            stream << "(" << hex(mCpu.mSystemBus.readByte(onePlusOpCodeAddress), 2)  << ")" << " ";
            stream << "                    [Direct Page Indirect]";
            break;
        case AddressingMode::DirectPageIndirectLong:
            stream << "[" << hex(mCpu.mSystemBus.readByte(mCpu.getAddressOfOpCodeData(opCode)), 2)  << "]" << " ";
            stream << "                    [Direct Page Indirect Long]";
            break;
        case AddressingMode::DirectPageIndexedIndirectWithX:
            stream << "(" << hex(mCpu.mSystemBus.readByte(
                                Address(mCpu.mProgramAddress.getBank(), mCpu.mProgramAddress.getOffset() + 1)), 2)  << ", X)" << " ";
            stream << "                    [Direct Page Indexed Indirect, X]";
            break;
        case AddressingMode::DirectPageIndirectIndexedWithY:
            stream << "(" << hex(mCpu.mSystemBus.readByte(
                                 Address(mCpu.mProgramAddress.getBank(), mCpu.mProgramAddress.getOffset() + 1)), 2)  << "), Y" << " ";
            stream << "                    [Direct Page Indirect Indexed, Y]";
            break;
        case AddressingMode::DirectPageIndirectLongIndexedWithY:
            stream << "[" << hex(mCpu.mSystemBus.readByte(mCpu.getAddressOfOpCodeData(opCode)), 2)  << "], Y" << " ";
            stream << "                    [Direct Page Indirect Indexed, Y]";
            break;
        case AddressingMode::StackImplied:
            stream << "                          [Stack Implied]";
            break;
        case AddressingMode::StackRelative:
            stream << hex(mCpu.mSystemBus.readByte(mCpu.getAddressOfOpCodeData(opCode)), 2)  << ", S" << " ";
            stream << "                    [Stack Relative]";
            break;
        case AddressingMode::StackAbsolute:
            break;
        case AddressingMode::StackDirectPageIndirect:
            break;
        case AddressingMode::StackProgramCounterRelativeLong:
            break;
        case AddressingMode::StackRelativeIndirectIndexedWithY:
            stream << "("
                   << hex(mCpu.mSystemBus.readByte(Address::sumOffsetToAddressWrapAround(mCpu.mProgramAddress, 1)), 2);
            stream << ", S), Y" << " ";
            stream << "                    [Absolute Indexed, X]";
            break;
        case AddressingMode::ProgramCounterRelative:
            stream << hex(mCpu.mSystemBus.readByte(mCpu.getAddressOfOpCodeData(opCode)), 2) << " ";
            stream << "                      [Program Counter Relative]";
            break;
        case AddressingMode::ProgramCounterRelativeLong:
            break;
        case AddressingMode::BlockMove:
            break;
    }

    std::cout << stream.str() << std::endl;
}

