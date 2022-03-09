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

#include "CpuStatus.h"
#include "Log.h"
#include "Binary.h"

#define LOG_TAG "CpuStatus"

/* **********************************************
 * 
 * Flags:
 * 
 *  b flag: the break flag
 *  c flag: the carry flag
 *  d flag: the decimal mode flag
 *  e flag: the emulation mode flag
 *  i flag: the interrupt disable flag
 *  m flag: the accumulator and memory width flag
 *  n flag: the negative flag
 *  v flag: the overflow flag
 *  x flag: the index register width flag
 *  z flag: the zero flag
 * 
 * **********************************************/

#define STATUS_CARRY                    0x01
#define STATUS_ZERO                     0x02
#define STATUS_INTERRUPT_DISABLE        0x04
#define STATUS_DECIMAL                  0x08

// In emulation mode
#define STATUS_BREAK                    0X10
// In native mode (x = 0, 16 bit)
#define STATUS_INDEX_WIDTH              0X10
// Only used in native mode
#define STATUS_ACCUMULATOR_WIDTH        0X20

#define STATUS_OVERFLOW                 0X40
#define STATUS_SIGN                     0X80

CpuStatus::CpuStatus() {
}

void CpuStatus::setZeroFlag() {
    mZeroFlag = true;
}

void CpuStatus::setSignFlag() {
    mSignFlag = true;
}

void CpuStatus::setDecimalFlag() {
    mDecimalFlag = true;
}

void CpuStatus::setInterruptDisableFlag() {
    mInterruptDisableFlag = true;
}

void CpuStatus::setAccumulatorWidthFlag() {
    mAccumulatorWidthFlag = true;
}

void CpuStatus::setIndexWidthFlag() {
    mIndexWidthFlag = true;
}

void CpuStatus::setCarryFlag() {
    mCarryFlag = true;
}

void CpuStatus::setEmulationFlag() {
    mEmulationFlag = true;
}

void CpuStatus::clearZeroFlag() {
    mZeroFlag = false;
}

void CpuStatus::clearSignFlag() {
    mSignFlag = false;
}

void CpuStatus::clearDecimalFlag() {
    mDecimalFlag = false;
}

void CpuStatus::clearInterruptDisableFlag() {
    mInterruptDisableFlag = false;
}

void CpuStatus::clearAccumulatorWidthFlag() {
    mAccumulatorWidthFlag = false;
}

void CpuStatus::clearIndexWidthFlag() {
    mIndexWidthFlag = false;
}

void CpuStatus::clearCarryFlag() {
    mCarryFlag = false;
}

void CpuStatus::clearEmulationFlag() {
    mEmulationFlag = false;
}

bool CpuStatus::zeroFlag() {
    return mZeroFlag;
}

bool CpuStatus::signFlag() {
    return mSignFlag;
}

bool CpuStatus::decimalFlag() {
    return mDecimalFlag;
}

bool CpuStatus::interruptDisableFlag() {
    return mInterruptDisableFlag;
}

bool CpuStatus::accumulatorWidthFlag() {
    return mAccumulatorWidthFlag;
}

bool CpuStatus::indexWidthFlag() {
    return mIndexWidthFlag;
}

bool CpuStatus::carryFlag() {
    return mCarryFlag;
}

bool CpuStatus::emulationFlag() {
    return mEmulationFlag;
}

void CpuStatus::setBreakFlag() {
    mBreakFlag = true;
}

void CpuStatus::clearBreakFlag() {
    mBreakFlag = false;
}

bool CpuStatus::breakFlag() {
    return mBreakFlag;
}

void CpuStatus::setOverflowFlag() {
    mOverflowFlag = true;
}

void CpuStatus::clearOverflowFlag() {
    mOverflowFlag = false;
}

bool CpuStatus::overflowFlag() {
    return mOverflowFlag;
}

uint8_t CpuStatus::getRegisterValue() {
    uint8_t value = 0;
    if (carryFlag()) value |= STATUS_CARRY;
    if (zeroFlag()) value |= STATUS_ZERO;
    if (interruptDisableFlag()) value |= STATUS_INTERRUPT_DISABLE;
    if (decimalFlag()) value |= STATUS_DECIMAL;
    if (emulationFlag() && breakFlag()) value |= STATUS_BREAK;
    if (!emulationFlag() && indexWidthFlag()) value |= STATUS_INDEX_WIDTH;
    if (!emulationFlag() && accumulatorWidthFlag()) value |= STATUS_ACCUMULATOR_WIDTH;
    if (overflowFlag()) value |= STATUS_OVERFLOW;
    if (signFlag()) value |= STATUS_SIGN;

    return value;
}

void CpuStatus::setRegisterValue(uint8_t value) {
    if (value & STATUS_CARRY) setCarryFlag();
    else clearCarryFlag();

    if (value & STATUS_ZERO) setZeroFlag();
    else clearZeroFlag();

    if (value & STATUS_INTERRUPT_DISABLE) setInterruptDisableFlag();
    else clearInterruptDisableFlag();

    if (value & STATUS_DECIMAL) setDecimalFlag();
    else clearDecimalFlag();

    if (emulationFlag()) {
        if (value & STATUS_BREAK) setBreakFlag();
        else clearBreakFlag();
    } else {
        if (value & STATUS_INDEX_WIDTH) setIndexWidthFlag();
        else clearIndexWidthFlag();
    }

    if (!emulationFlag() && (value & STATUS_ACCUMULATOR_WIDTH)) setAccumulatorWidthFlag();
    else clearAccumulatorWidthFlag();

    if (value & STATUS_OVERFLOW) setOverflowFlag();
    else clearOverflowFlag();

    if (value & STATUS_SIGN) setSignFlag();
    else clearSignFlag();
}

void CpuStatus::updateZeroFlagFrom8BitValue(uint8_t value) {
    if (Binary::is8bitValueZero(value)) setZeroFlag();
    else clearZeroFlag();
}

void CpuStatus::updateZeroFlagFrom16BitValue(uint16_t value) {
    if (Binary::is16bitValueZero(value)) setZeroFlag();
    else clearZeroFlag();
}

void CpuStatus::updateSignFlagFrom8BitValue(uint8_t value) {
    if (Binary::is8bitValueNegative(value)) setSignFlag();
    else clearSignFlag();
}

void CpuStatus::updateSignFlagFrom16BitValue(uint16_t value) {
    if (Binary::is16bitValueNegative(value)) setSignFlag();
    else clearSignFlag();
}

void CpuStatus::updateSignAndZeroFlagFrom8BitValue(uint8_t value) {
    updateSignFlagFrom8BitValue(value);
    updateZeroFlagFrom8BitValue(value);
}

void CpuStatus::updateSignAndZeroFlagFrom16BitValue(uint16_t value) {
    updateSignFlagFrom16BitValue(value);
    updateZeroFlagFrom16BitValue(value);
}
