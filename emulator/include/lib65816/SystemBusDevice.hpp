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

#ifndef SYSBUS_DEVICE_H
#define SYSBUS_DEVICE_H

#include <stdint.h>

#define BANK_SIZE_BYTES                0x10000
#define HALF_BANK_SIZE_BYTES            0x8000
#define PAGE_SIZE_BYTES                    256

class Address {
    private:
        uint8_t mBank;
        uint16_t mOffset;

    public:
        static bool offsetsAreOnDifferentPages(uint16_t, uint16_t);
        static Address sumOffsetToAddress(const Address &, uint16_t);
        static Address sumOffsetToAddressNoWrapAround(const Address &, uint16_t);
        static Address sumOffsetToAddressWrapAround(const Address &, uint16_t);

        Address() = default;
        Address(uint8_t bank, uint16_t offset) : mBank(bank), mOffset(offset) {};

        Address newWithOffset(uint16_t);
        Address newWithOffsetNoWrapAround(uint16_t);
        Address newWithOffsetWrapAround(uint16_t);

        void incrementOffsetBy(uint16_t);
        void decrementOffsetBy(uint16_t);

        void getBankAndOffset(uint8_t *bank, uint16_t *offset) {
            *bank = mBank;
            *offset = mOffset;
        }

        uint8_t getBank() const {
            return mBank;
        }

        uint16_t getOffset() const {
            return mOffset;
        }
};

/**
 Every device (PPU, APU, ...) implements this interface.
 */
class SystemBusDevice {
    public:
        virtual ~SystemBusDevice() {};

        /**
          Stores one byte to the real address represented by the specified virtual address.
          That is: maps the virtual address to the real one and stores one byte in it.
         */
        virtual void storeByte(const Address &, uint8_t) = 0;

        /**
          Reads one byte from the real address represented by the specified virtual address.
          That is: maps the virtual address to the real one and reads from it.
         */
        virtual uint8_t readByte(const Address &) = 0;

        /**
          Returns true if the address was decoded successfully by this device.
         */
        virtual bool decodeAddress(const Address &, Address &) = 0;
};

#endif // SYSBUS_DEVICE_H
