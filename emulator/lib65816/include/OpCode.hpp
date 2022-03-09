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

#ifndef OPCODE_HPP
#define OPCODE_HPP

#include <stdint.h>

#include "Addressing.hpp"

class Cpu65816;

class OpCode {
    private:
        uint8_t mCode;
        const char * const mName;
        AddressingMode mAddressingMode;
        void (Cpu65816::*mExecutor)(OpCode &);

    public:
        OpCode(uint8_t code, const char * const name, const AddressingMode &addressingMode) :
            mCode(code), mName(name), mAddressingMode(addressingMode), mExecutor(0) {
        }

        OpCode(uint8_t code, const char * const name, const AddressingMode &addressingMode, void (Cpu65816::*executor)(OpCode &)) :
            mCode(code), mName(name), mAddressingMode(addressingMode), mExecutor(executor) {
        }

        const uint8_t getCode() {
            return mCode;
        }

        const char *getName() {
            return mName;
        }

        const AddressingMode getAddressingMode() {
            return mAddressingMode;
        }

        const bool execute(Cpu65816 &cpu) {
            if (mExecutor != 0) {
                OpCode opCode = *this;
                (cpu.*mExecutor)(opCode);
                return true;
            }
            return false;
        }
};

#endif // OPCODE_HPP
