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

#include <cstdint>

#ifndef EMULATOR_BINARY_H
#define EMULATOR_BINARY_H

namespace Binary {
    uint8_t lower8BitsOf(uint16_t);
    uint8_t higher8BitsOf(uint16_t);
    uint16_t lower16BitsOf(uint32_t);
    bool is8bitValueNegative(uint8_t);
    bool is16bitValueNegative(uint16_t);
    bool is8bitValueZero(uint8_t);
    bool is16bitValueZero(uint16_t);
    void setLower8BitsOf16BitsValue(uint16_t *, uint8_t);
    void setBitIn8BitValue(uint8_t *, uint8_t);
    void clearBitIn8BitValue(uint8_t *, uint8_t);
    void setBitIn16BitValue(uint16_t *, uint8_t);
    void clearBitIn16BitValue(uint16_t *, uint8_t);

    uint8_t convert8BitToBcd(uint8_t);
    uint16_t convert16BitToBcd(uint16_t);
    bool bcdSum8Bit(uint8_t, uint8_t, uint8_t *, bool);
    bool bcdSum16Bit(uint16_t, uint16_t, uint16_t *, bool);
    bool bcdSubtract8Bit(uint8_t, uint8_t, uint8_t *, bool);
    bool bcdSubtract16Bit(uint16_t, uint16_t, uint16_t *, bool);
}

#endif // EMULATOR_BINARY_H