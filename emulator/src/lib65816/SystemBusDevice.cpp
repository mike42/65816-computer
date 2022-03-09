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

#include <cmath>

#include "SystemBusDevice.hpp"

Address Address::sumOffsetToAddressNoWrapAround(const Address &address, uint16_t offset) {
    uint8_t newBank = address.getBank();
    uint16_t newOffset;
    auto offsetSum = (uint32_t)(address.getOffset() + offset);
    if (offsetSum >= BANK_SIZE_BYTES) {
        ++newBank;
        newOffset = static_cast<uint16_t>(offsetSum - BANK_SIZE_BYTES);
    } else {
        newOffset = address.getOffset() + offset;
    }
    return {newBank, newOffset};
}

Address Address::sumOffsetToAddressWrapAround(const Address &address, uint16_t offset) {
    return Address(address.getBank(), address.getOffset() + offset);
}

Address Address::sumOffsetToAddress(const Address &address, uint16_t offset) {
    // This wraps around by default
    // TODO figure out when to wrap around and when not to
    return sumOffsetToAddressWrapAround(address, offset);
}

bool Address::offsetsAreOnDifferentPages(uint16_t offsetFirst, uint16_t offsetSecond) {
    int pageOfFirst = static_cast<int>(std::floor(offsetFirst / PAGE_SIZE_BYTES));
    int pageOfSecond = static_cast<int>(std::floor(offsetSecond / PAGE_SIZE_BYTES));
    return pageOfFirst != pageOfSecond;
}

Address Address::newWithOffset(uint16_t offset) {
    return sumOffsetToAddress((const Address &)*this, offset);
}

Address Address::newWithOffsetNoWrapAround(uint16_t offset) {
    return sumOffsetToAddressNoWrapAround((const Address &)*this, offset);
}

Address Address::newWithOffsetWrapAround(uint16_t offset) {
    return sumOffsetToAddressWrapAround((const Address &)*this, offset);
}

void Address::decrementOffsetBy(uint16_t offset) {
    mOffset -= offset;
}

void Address::incrementOffsetBy(uint16_t offset) {
    mOffset += offset;
}
