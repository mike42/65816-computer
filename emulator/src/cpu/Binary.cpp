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

#include "Binary.h"

namespace Binary {

    uint8_t lower8BitsOf(uint16_t value) {
        return ((uint8_t) (value & 0xFF));
    }

    uint8_t higher8BitsOf(uint16_t value) {
        return ((uint8_t) ((value & 0xFF00) >> 8));
    }

    uint16_t lower16BitsOf(uint32_t value) {
        return ((uint16_t) (value & 0xFFFF));
    }

    bool is8bitValueNegative(uint8_t value) {
        return (value & 0x80);
    }

    bool is16bitValueNegative(uint16_t value) {
        return (value & 0x8000);
    }

    bool is8bitValueZero(uint8_t value) {
        return (value == 0x00);
    }

    bool is16bitValueZero(uint16_t value) {
        return (value == 0x0000);
    }

    void setLower8BitsOf16BitsValue(uint16_t *destination, uint8_t value) {
        *destination &= 0xFF00;
        *destination |= value;
    }

    void setBitIn8BitValue(uint8_t *value, uint8_t bitNumber) {
        auto mask = static_cast<uint8_t>(1 << bitNumber);
        *value = *value | mask;
    }

    void clearBitIn8BitValue(uint8_t *value, uint8_t bitNumber) {
        auto mask = static_cast<uint8_t>(1 << bitNumber);
        mask = static_cast<uint8_t>(mask ^ 0xFF);
        *value = *value & mask;
    }

    void setBitIn16BitValue(uint16_t *value, uint8_t bitNumber) {
        auto mask = static_cast<uint16_t>(1 << bitNumber);
        *value = *value | mask;
    }

    void clearBitIn16BitValue(uint16_t *value, uint8_t bitNumber) {
        auto mask = static_cast<uint16_t>(1 << bitNumber);
        mask = static_cast<uint16_t>(mask ^ 0xFFFF);
        *value = *value & mask;
    }

    uint8_t convert8BitToBcd(uint8_t val) {
        uint8_t value = val;
        uint8_t result = 0;
        uint8_t shiftLeft = 0;
        while (value > 0) {
            auto digit = static_cast<uint8_t>(value % 10);
            result |= digit << shiftLeft;
            value /= 10;
            shiftLeft += 4;
        }

        return result;
    }

    uint16_t convert16BitToBcd(uint16_t val) {
        uint16_t value = val;
        uint16_t result = 0;
        uint16_t shiftLeft = 0;
        while (value > 0) {
            auto digit = static_cast<uint8_t>(value % 10);
            result |= digit << shiftLeft;
            value /= 10;
            shiftLeft += 4;
        }

        return result;
    }

    bool bcdSum8Bit(uint8_t bcdFirst, uint8_t bcdSecond, uint8_t *result, bool carry) {
        uint8_t shift = 0;
        *result = 0;

        while (shift < 8) {
            auto digitOfFirst = static_cast<uint8_t>(bcdFirst & 0xF);
            auto digitOfSecond = static_cast<uint8_t>(bcdSecond & 0xF);
            auto sumOfDigits = static_cast<uint8_t>(digitOfFirst + digitOfSecond + (carry ? 1 : 0));
            carry = sumOfDigits > 9;
            if (carry) sumOfDigits += 6;
            sumOfDigits &= 0xF;
            *result |= sumOfDigits << shift;

            shift += 4;
            bcdFirst >>= shift;
            bcdSecond >>= shift;
        }

        return carry;
    }

    bool bcdSubtract8Bit(uint8_t bcdFirst, uint8_t bcdSecond, uint8_t *result, bool borrow) {
        uint8_t shift = 0;
        *result = 0;

        while (shift < 8) {
            auto digitOfFirst = static_cast<uint8_t>(bcdFirst & 0xF);
            auto digitOfSecond = static_cast<uint8_t>(bcdSecond & 0xF);
            auto diffOfDigits = static_cast<uint8_t>(digitOfFirst - digitOfSecond - (borrow ? 1 : 0));
            borrow = diffOfDigits > 9;
            if (borrow) diffOfDigits -= 6;
            diffOfDigits &= 0xF;
            *result |= diffOfDigits << shift;

            shift += 4;
            bcdFirst >>= shift;
            bcdSecond >>= shift;
        }

        return borrow;
    }

    bool bcdSum16Bit(uint16_t bcdFirst, uint16_t bcdSecond, uint16_t *result, bool carry) {
        *result = 0;
        uint8_t shift = 0;
        while (shift < 16) {
            auto digitOfFirst = static_cast<uint8_t>(bcdFirst & 0xFF);
            auto digitOfSecond = static_cast<uint8_t>(bcdSecond & 0xFF);
            uint8_t partialresult = 0;
            carry = bcdSum8Bit(digitOfFirst, digitOfSecond, &partialresult, carry);
            *result |= partialresult << shift;
            shift += 8;
            bcdFirst >>= shift;
            bcdSecond >>= shift;
        }
        return carry;
    }

    bool bcdSubtract16Bit(uint16_t bcdFirst, uint16_t bcdSecond, uint16_t *result, bool borrow) {
        *result = 0;
        uint8_t shift = 0;
        while (shift < 16) {
            auto digitOfFirst = static_cast<uint8_t>(bcdFirst & 0xFF);
            auto digitOfSecond = static_cast<uint8_t>(bcdSecond & 0xFF);
            uint8_t partialresult = 0;
            borrow = bcdSubtract8Bit(digitOfFirst, digitOfSecond, &partialresult, borrow);
            *result |= partialresult << shift;
            shift += 8;
            bcdFirst >>= shift;
            bcdSecond >>= shift;
        }
        return borrow;
    }

}
