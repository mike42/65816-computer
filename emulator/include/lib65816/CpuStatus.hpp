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

#ifndef CPUSTATUS_H
#define CPUSTATUS_H

#include <stdint.h>

class CpuStatus {
    public:
        CpuStatus();
    
        void setZeroFlag();
        void clearZeroFlag();
        bool zeroFlag();
        
        void setSignFlag();
        void clearSignFlag();
        bool signFlag();
        
        void setDecimalFlag();
        void clearDecimalFlag();
        bool decimalFlag();
        
        void setInterruptDisableFlag();
        void clearInterruptDisableFlag();
        bool interruptDisableFlag();
        
        void setAccumulatorWidthFlag();
        void clearAccumulatorWidthFlag();
        bool accumulatorWidthFlag();

        void setIndexWidthFlag();
        void clearIndexWidthFlag();
        bool indexWidthFlag();
        
        void setCarryFlag();
        void clearCarryFlag();
        bool carryFlag();
        
        void setBreakFlag();
        void clearBreakFlag();
        bool breakFlag();
        
        void setOverflowFlag();
        void clearOverflowFlag();
        bool overflowFlag();
        
        void setEmulationFlag();
        void clearEmulationFlag();
        bool emulationFlag();
        
        uint8_t getRegisterValue();
        void setRegisterValue(uint8_t);
        
        void updateZeroFlagFrom8BitValue(uint8_t);
        void updateZeroFlagFrom16BitValue(uint16_t);
        void updateSignFlagFrom8BitValue(uint8_t);
        void updateSignFlagFrom16BitValue(uint16_t);
        void updateSignAndZeroFlagFrom8BitValue(uint8_t);
        void updateSignAndZeroFlagFrom16BitValue(uint16_t);
    
    private:
        bool mZeroFlag = false;
        bool mSignFlag = false;
        bool mDecimalFlag = false;
        bool mInterruptDisableFlag = false;
        bool mAccumulatorWidthFlag = false;
        bool mIndexWidthFlag = false;
        bool mCarryFlag = false;
        bool mEmulationFlag = true; // CPU Starts in emulation mode
        bool mOverflowFlag = false;
        bool mBreakFlag = false;
};

#endif // CPUSTATUS_H
