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

#include <utility>

#ifdef EMU_65C02
#define LOG_TAG "Cpu65C02"
#else
#define LOG_TAG (mCpuStatus.emulationFlag() ? "Cpu6502" : "Cpu65816")
#endif

Cpu65816::Cpu65816(SystemBus &systemBus, std::shared_ptr<InterruptStatus> nmi, std::shared_ptr<InterruptStatus> irq) :
        mSystemBus(systemBus),
        mStack(&mSystemBus),
        nmi(nmi),
        irq(irq) {
    mPins.NMI = nmi->get();
}


void Cpu65816::setXL(uint8_t x) {
    mX = x;
}

void Cpu65816::setYL(uint8_t y) {
    mY = y;
}

void Cpu65816::setX(uint16_t x) {
    mX = x;
}

void Cpu65816::setY(uint16_t y) {
    mY = y;
}

void Cpu65816::setA(uint16_t a) {
    mA = a;
}

uint16_t Cpu65816::getA() {
    return mA;
}

Address Cpu65816::getProgramAddress() {
    return mProgramAddress;
}

Stack *Cpu65816::getStack() {
    return &mStack;
}

CpuStatus *Cpu65816::getCpuStatus() {
    return &mCpuStatus;
}


/**
 * Resets the cpu to its initial state.
 * */
void Cpu65816::reset() {
    setRESPin(true);
    mCpuStatus.setEmulationFlag();
    mCpuStatus.setAccumulatorWidthFlag();
    mCpuStatus.setIndexWidthFlag();
    mX &= 0xFF;
    mY &= 0xFF;
    mD = 0x0;
    mStack = Stack(&mSystemBus);
    mProgramAddress = Address(0x00, mSystemBus.readTwoBytes(Address(0x00, INTERRUPT_VECTOR_EMULATION_RESET)));
}

void Cpu65816::setRESPin(bool value) {
    if (value == false && mPins.RES == true) {
        reset();
    }
    mPins.RES = value;
}

void Cpu65816::setRDYPin(bool value) {
    mPins.RDY = value;
}

bool Cpu65816::executeNextInstruction() {
    if (mPins.RES) {
        return false;
    }
    if ((irq->get()) && (!mCpuStatus.interruptDisableFlag())) {
        /*
        The program bank register (PB, the A16-A23 part of the address bus) is pushed onto the hardware stack (65C816/65C802 only when operating in native mode).
        The most significant byte (MSB) of the program counter (PC) is pushed onto the stack.
        The least significant byte (LSB) of the program counter is pushed onto the stack.
        The status register (SR) is pushed onto the stack.
        The interrupt disable flag is set in the status register.
        PB is loaded with $00 (65C816/65C802 only when operating in native mode).
        PC is loaded from the relevant vector (see tables).
        */
        if (!mCpuStatus.emulationFlag()) {
            mStack.push8Bit(mProgramAddress.getBank());
            mStack.push16Bit(mProgramAddress.getOffset());
            mStack.push8Bit(mCpuStatus.getRegisterValue());
            mCpuStatus.setInterruptDisableFlag();
            mProgramAddress = Address(0x00, mSystemBus.readTwoBytes(Address(0x00, INTERRUPT_VECTOR_NATIVE_IRQ)));
        } else {
            mStack.push16Bit(mProgramAddress.getOffset());
            mStack.push8Bit(mCpuStatus.getRegisterValue());
            mCpuStatus.setInterruptDisableFlag();
            mProgramAddress = Address(0x00, mSystemBus.readTwoBytes(Address(0x00, INTERRUPT_VECTOR_EMULATION_BRK_IRQ)));
        }
    }
    // Edge detect NMI
    bool newNmi = nmi->get();
    if(!newNmi && mPins.NMI) {
        // De-activated
        mPins.NMI = false;
    } else if(newNmi && !mPins.NMI) {
        // Activated
        mPins.NMI = true;
        if (!mCpuStatus.emulationFlag()) {
            mStack.push8Bit(mProgramAddress.getBank());
            mStack.push16Bit(mProgramAddress.getOffset());
            mStack.push8Bit(mCpuStatus.getRegisterValue());
            mCpuStatus.setInterruptDisableFlag();
            mProgramAddress = Address(0x00, mSystemBus.readTwoBytes(Address(0x00, INTERRUPT_VECTOR_NATIVE_NMI)));
        } else {
            mStack.push16Bit(mProgramAddress.getOffset());
            mStack.push8Bit(mCpuStatus.getRegisterValue());
            mCpuStatus.setInterruptDisableFlag();
            mProgramAddress = Address(0x00, mSystemBus.readTwoBytes(Address(0x00, INTERRUPT_VECTOR_EMULATION_NMI)));
        }
    }
    // Fetch the instruction
    const uint8_t instruction = mSystemBus.readByte(mProgramAddress);
    OpCode opCode = OP_CODE_TABLE[instruction];
    // Execute it
    return opCode.execute(*this);
}

bool Cpu65816::accumulatorIs8BitWide() {
    // Accumulator is always 8 bit in emulation mode.
    if (mCpuStatus.emulationFlag()) return true;
        // Accumulator width set to one means 8 bit accumulator.
    else return mCpuStatus.accumulatorWidthFlag();
}

bool Cpu65816::accumulatorIs16BitWide() {
    return !accumulatorIs8BitWide();
}

bool Cpu65816::indexIs8BitWide() {
    // Index is always 8 bit in emulation mode.
    if (mCpuStatus.emulationFlag()) return true;
        // Index width set to one means 8 bit accumulator.
    else return mCpuStatus.indexWidthFlag();
}

bool Cpu65816::indexIs16BitWide() {
    return !indexIs8BitWide();
}

void Cpu65816::addToCycles(int cycles) {
    mTotalCyclesCounter += cycles;
    mSystemBus.clockTick(cycles);
}

void Cpu65816::subtractFromCycles(int cycles) {
    mTotalCyclesCounter -= cycles;
}

void Cpu65816::addToProgramAddress(int bytes) {
    mProgramAddress.incrementOffsetBy(bytes);
}

void Cpu65816::addToProgramAddressAndCycles(int bytes, int cycles) {
    addToCycles(cycles);
    addToProgramAddress(bytes);
}

uint16_t Cpu65816::indexWithXRegister() {
    return indexIs8BitWide() ? Binary::lower8BitsOf(mX) : mX;
}

uint16_t Cpu65816::indexWithYRegister() {
    return indexIs8BitWide() ? Binary::lower8BitsOf(mY) : mY;
}

void Cpu65816::setProgramAddress(const Address &address) {
    mProgramAddress = address;
}

uint64_t Cpu65816::getTotalCyclesCounter() const {
    return mTotalCyclesCounter;
}
