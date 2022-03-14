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

#ifndef EMULATOR_INTERRUPT_H
#define EMULATOR_INTERRUPT_H

#include <cstdint>

// Interrupt table. Native mode.
typedef struct {
    const uint16_t coProcessorEnable;
    const uint16_t brk;
    const uint16_t abort;
    const uint16_t nonMaskableInterrupt;
    const uint16_t reset;
    const uint16_t interruptRequest;
} NativeModeInterrupts;

// Interrupt table. Emulation mode.
typedef struct {
    const uint16_t coProcessorEnable;
    const uint16_t unused;
    const uint16_t abort;
    const uint16_t nonMaskableInterrupt;
    const uint16_t reset;
    const uint16_t brkIrq;
} EmulationModeInterrupts;

#endif // EMULATOR_INTERRUPT_H
