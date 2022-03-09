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

#ifndef EMULATOR_SYSTEMBUS_H
#define EMULATOR_SYSTEMBUS_H

#include <cstdint>
#include <vector>

#include "SystemBusDevice.h"

class SystemBus {
public:
    void registerDevice(SystemBusDevice *);
    void storeByte(const Address &, uint8_t);
    void storeTwoBytes(const Address &, uint16_t);
    uint8_t readByte(const Address &);
    uint16_t readTwoBytes(const Address &);
    Address readAddressAt(const Address &);

private:

    std::vector<SystemBusDevice *> mDevices;
};

#endif // EMULATOR_SYSTEMBUS_H
