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

#include "Stack.h"
#include "Log.h"

#define LOG_TAG "Stack"

Stack::Stack(SystemBus *systemBus) :
        mSystemBus(systemBus),
        mStackAddress(0x00, STACK_POINTER_DEFAULT) {
    Log::trc(LOG_TAG).str("Initialized at default location ").sp().hex(mStackAddress.getOffset(), 4).show();
}

Stack::Stack(SystemBus *systemBus, uint16_t stackPointer) :
        mSystemBus(systemBus),
        mStackAddress(0x00, stackPointer) {
    Log::trc(LOG_TAG).str("Initialized at location ").sp().hex(mStackAddress.getOffset(), 4).show();
}

void Stack::push8Bit(uint8_t value) {
    mSystemBus->storeByte(mStackAddress, value);
    mStackAddress.decrementOffsetBy(sizeof(uint8_t));
}

void Stack::push16Bit(uint16_t value) {
    auto leastSignificant = (uint8_t) ((value) & 0xFF);
    auto mostSignificant = (uint8_t) (((value) & 0xFF00) >> 8);
    push8Bit(mostSignificant);
    push8Bit(leastSignificant);
}

uint8_t Stack::pull8Bit() {
    mStackAddress.incrementOffsetBy(sizeof(uint8_t));
    return mSystemBus->readByte(mStackAddress);
}

uint16_t Stack::pull16Bit() {
    return (uint16_t) (pull8Bit() | (((uint16_t) pull8Bit()) << 8));
}

uint16_t Stack::getStackPointer() {
    return mStackAddress.getOffset();
}
