/*
 * This file is part of the simple-logger Library.
 * Copyright (c) 2018 Francesco Rigoni.
 *
 * https://github.com/FrancescoRigoni/simple-logger
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

#ifndef __LOG__
#define __LOG__

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <iomanip>

class Log {
    private:
        static Log sDebugLog;
        static Log sVerboseLog;
        static Log sTraceLog;
        static Log sErrorLog;
        
        Log(const bool);
        const char *mTag;
        const bool mEnabled;
        std::ostringstream mStream;
        
    public:
        // Returns debug log
        static Log& dbg(const char *);
        // Returns verbose log
        static Log& vrb(const char *);
        // Returns trace log
        static Log& trc(const char *);
        // Returns error log
        static Log& err(const char *);
        
        Log &str(const char *);
        Log &hex(uint32_t);
        Log &hex(uint32_t, uint8_t);
        Log &dec(uint32_t);
        Log &dec(uint32_t, uint8_t);
        Log &sp();
        
        void show();
};

#endif

