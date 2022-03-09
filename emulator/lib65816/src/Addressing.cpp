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

#include <Cpu65816.hpp>
#include <OpCode.hpp>
#include <SystemBusDevice.hpp>

#define LOG_TAG "Addressing"

bool Cpu65816::opCodeAddressingCrossesPageBoundary(OpCode &opCode) {
    switch(opCode.getAddressingMode()) {
        case AddressingMode::AbsoluteIndexedWithX:
        {
            Address initialAddress(mDB, mSystemBus.readTwoBytes(mProgramAddress.newWithOffset(1)));
            // TODO: figure out when to wrap around and when not to, it should not matter in this case
            // but it matters when fetching data
            Address finalAddress = Address::sumOffsetToAddress(initialAddress, indexWithXRegister());
            return Address::offsetsAreOnDifferentPages(initialAddress.getOffset(), finalAddress.getOffset());
        }
        case AddressingMode::AbsoluteIndexedWithY:
        {
            Address initialAddress(mDB, mSystemBus.readTwoBytes(mProgramAddress.newWithOffset(1)));
            // TODO: figure out when to wrap around and when not to, it should not matter in this case
            // but it matters when fetching data
            Address finalAddress = Address::sumOffsetToAddress(initialAddress, indexWithYRegister());
            return Address::offsetsAreOnDifferentPages(initialAddress.getOffset(), finalAddress.getOffset());
        }
        case AddressingMode::DirectPageIndirectIndexedWithY:
        {
            uint16_t firstStageOffset = mD + mSystemBus.readByte(mProgramAddress.newWithOffset(1));
            Address firstStageAddress(0x00, firstStageOffset);
            uint16_t secondStageOffset = mSystemBus.readTwoBytes(firstStageAddress);
            Address thirdStageAddress(mDB, secondStageOffset);
            // TODO: figure out when to wrap around and when not to, it should not matter in this case
            // but it matters when fetching data
            Address finalAddress = Address::sumOffsetToAddress(thirdStageAddress, indexWithYRegister());
            return Address::offsetsAreOnDifferentPages(thirdStageAddress.getOffset(), finalAddress.getOffset());
        }
        default:
        {
            Log::err(LOG_TAG).str("!!! Unsupported opCodeAddressingCrossesPageBoundary for opCode: ").hex(opCode.getCode(), 2).show();
        }

    }

    return false;
}

Address Cpu65816::getAddressOfOpCodeData(OpCode &opCode) {
    uint8_t dataAddressBank;
    uint16_t dataAddressOffset;

    switch(opCode.getAddressingMode()) {
        case AddressingMode::Interrupt:
        case AddressingMode::Accumulator:
        case AddressingMode::Implied:
        case AddressingMode::StackImplied:
            // Not really used, doesn't make any sense since these opcodes do not have operands
            return mProgramAddress;
        case AddressingMode::Immediate:
        case AddressingMode::BlockMove:
            // Blockmove OpCodes have two bytes following them directly
        case AddressingMode::StackAbsolute:
            // Stack absolute is used to push values following the op code onto the stack
        case AddressingMode::ProgramCounterRelative:
            // Program counter relative OpCodes such as all branch instructions have an 8 bit operand
            // following the op code
        case AddressingMode::ProgramCounterRelativeLong:
            // StackProgramCounterRelativeLong is only used by the PER OpCode, it has 16 bit operand
        case AddressingMode::StackProgramCounterRelativeLong:
            mProgramAddress.newWithOffset(1).getBankAndOffset(&dataAddressBank, &dataAddressOffset);
            break;
        case AddressingMode::Absolute:
            dataAddressBank = mDB;
            dataAddressOffset = mSystemBus.readTwoBytes(mProgramAddress.newWithOffset(1));
            break;
        case AddressingMode::AbsoluteLong:
            mSystemBus.readAddressAt(mProgramAddress.newWithOffset(1)).getBankAndOffset(&dataAddressBank, &dataAddressOffset);
            break;
        case AddressingMode::AbsoluteIndirect:
        {
            dataAddressBank = mProgramAddress.getBank();
            Address addressOfOffset(0x00, mSystemBus.readTwoBytes(mProgramAddress.newWithOffset(1)));
            dataAddressOffset = mSystemBus.readTwoBytes(addressOfOffset);
        }
            break;
        case AddressingMode::AbsoluteIndirectLong:
        {
            Address addressOfEffectiveAddress(0x00, mSystemBus.readTwoBytes(mProgramAddress.newWithOffset(1)));
            mSystemBus.readAddressAt(addressOfEffectiveAddress).getBankAndOffset(&dataAddressBank, &dataAddressOffset);
        }
            break;
        case AddressingMode::AbsoluteIndexedIndirectWithX:
        {
            Address firstStageAddress(mProgramAddress.getBank(), mSystemBus.readTwoBytes(mProgramAddress.newWithOffset(1)));
            Address secondStageAddress = firstStageAddress.newWithOffsetNoWrapAround(indexWithXRegister());
            dataAddressBank = mProgramAddress.getBank();
            dataAddressOffset = mSystemBus.readTwoBytes(secondStageAddress);
        }
            break;
        case AddressingMode::AbsoluteIndexedWithX:
        {
            Address firstStageAddress(mDB, mSystemBus.readTwoBytes(mProgramAddress.newWithOffset(1)));
            Address::sumOffsetToAddressNoWrapAround(firstStageAddress, indexWithXRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);;
        }
            break;
        case AddressingMode::AbsoluteLongIndexedWithX:
        {
            Address firstStageAddress = mSystemBus.readAddressAt(mProgramAddress.newWithOffset(1));
            Address::sumOffsetToAddressNoWrapAround(firstStageAddress, indexWithXRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);;
        }
            break;
        case AddressingMode::AbsoluteIndexedWithY:
        {
            Address firstStageAddress(mDB, mSystemBus.readTwoBytes(mProgramAddress.newWithOffset(1)));
            Address::sumOffsetToAddressNoWrapAround(firstStageAddress, indexWithYRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);;
        }
            break;
        case AddressingMode::DirectPage:
        {
            // Direct page/Zero page always refers to bank zero
            dataAddressBank = 0x00;
            if (mCpuStatus.emulationFlag()) {
                // 6502 uses zero page
                dataAddressOffset = mSystemBus.readByte(mProgramAddress.newWithOffset(1));
            } else {
                // 65816 uses direct page
                dataAddressOffset = mD + mSystemBus.readByte(mProgramAddress.newWithOffset(1));
            }
        }
            break;
        case AddressingMode::DirectPageIndexedWithX:
        {
            dataAddressBank = 0x00;
            dataAddressOffset = mD + indexWithXRegister() + mSystemBus.readByte(mProgramAddress.newWithOffset(1));
        }
            break;
        case AddressingMode::DirectPageIndexedWithY:
        {
            dataAddressBank = 0x00;
            dataAddressOffset = mD + indexWithYRegister() + mSystemBus.readByte(mProgramAddress.newWithOffset(1));
        }
            break;
        case AddressingMode::DirectPageIndirect:
        {
            Address firstStageAddress(0x00, mD + mSystemBus.readByte(mProgramAddress.newWithOffset(1)));
            dataAddressBank = mDB;
            dataAddressOffset = mSystemBus.readTwoBytes(firstStageAddress);
        }
            break;
        case AddressingMode::DirectPageIndirectLong:
        {
            Address firstStageAddress(0x00, mD + mSystemBus.readByte(mProgramAddress.newWithOffset(1)));
            mSystemBus.readAddressAt(firstStageAddress)
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);;
        }
            break;
        case AddressingMode::DirectPageIndexedIndirectWithX:
        {
            Address firstStageAddress(0x00, mD + mSystemBus.readByte(mProgramAddress.newWithOffset(1)) + indexWithXRegister());
            dataAddressBank = mDB;
            dataAddressOffset = mSystemBus.readTwoBytes(firstStageAddress);
        }
            break;
        case AddressingMode::DirectPageIndirectIndexedWithY:
        {
            Address firstStageAddress(0x00, mD + mSystemBus.readByte(mProgramAddress.newWithOffset(1)));
            uint16_t secondStageOffset = mSystemBus.readTwoBytes(firstStageAddress);
            Address thirdStageAddress(mDB, secondStageOffset);
            Address::sumOffsetToAddressNoWrapAround(thirdStageAddress, indexWithYRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);
        }
            break;
        case AddressingMode::DirectPageIndirectLongIndexedWithY:
        {
            Address firstStageAddress(0x00, mD + mSystemBus.readByte(mProgramAddress.newWithOffset(1)));
            Address secondStageAddress = mSystemBus.readAddressAt(firstStageAddress);
            Address::sumOffsetToAddressNoWrapAround(secondStageAddress, indexWithYRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);
        }
            break;
        case AddressingMode::StackRelative:
        {
            dataAddressBank = 0x00;
            dataAddressOffset = mStack.getStackPointer() + mSystemBus.readByte(mProgramAddress.newWithOffset(1));
        }
            break;
        case AddressingMode::StackDirectPageIndirect:
        {
            dataAddressBank = 0x00;
            dataAddressOffset = mD + mSystemBus.readByte(mProgramAddress.newWithOffset(1));
        }
            break;
        case AddressingMode::StackRelativeIndirectIndexedWithY:
        {
            Address firstStageAddress(0x00, mStack.getStackPointer() + mSystemBus.readByte(mProgramAddress.newWithOffset(1)));
            uint16_t secondStageOffset = mSystemBus.readTwoBytes(firstStageAddress);
            Address thirdStageAddress(mDB, secondStageOffset);
            Address::sumOffsetToAddressNoWrapAround(thirdStageAddress, indexWithYRegister())
                .getBankAndOffset(&dataAddressBank, &dataAddressOffset);
        }
            break;
    }

    return Address(dataAddressBank, dataAddressOffset);
}
