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

#ifndef EMULATOR_CPU65816DEBUGGER_H
#define EMULATOR_CPU65816DEBUGGER_H

#include <cstdint>
#include <functional>

#include "DebugSymbols.h"
#include "SystemBusDevice.h"
#include "BuildConfig.h"
#include "Cpu65816.h"

class Cpu65816Debugger {
public:
    Cpu65816Debugger(Cpu65816 &, DebugSymbols &);

    void step();
    void setBreakPoint(const Address &);
    void dumpCpu() const;
    void logStatusRegister() const;
    void logOpCode(OpCode &) const;

    void doBeforeStep(std::function<void()>);
    void doAfterStep(std::function<void()>);
    void onBreakPoint(std::function<void()>);
    void onStp(std::function<void()>);

private:
    std::function<void()> mOnBeforeStepHandler;
    std::function<void()> mOnAfterStepHandler;
    std::function<void()> mOnBreakPointHandler;
    std::function<void()> mStpHandler;

    // Let's assume $00:$0000 is not a valid address for code
    Address mBreakPointAddress{0x00, 0x0000};
    bool mBreakpointHit = false;

    Cpu65816 &mCpu;
    DebugSymbols &mSymbols;
};

#endif // EMULATOR_CPU65816DEBUGGER_H
