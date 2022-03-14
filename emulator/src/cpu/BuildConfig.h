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

/**
 * The following define enables building the Cpu65816 for 65C02 emulation.
 * Note that this will enable 65C02 emulation but NOT R65C02 emulation.
 * The R65C02 cpu has several different opcodes and is not compatible
 * with the 65C02.
 *
 * This define enables some minor differences in cycles counting
 * for some opcodes.
 * */
//#define EMU_65C02