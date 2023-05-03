#ifndef EMULATOR_DEBUGSYMBOLS_H
#define EMULATOR_DEBUGSYMBOLS_H

#include "string"

class DebugSymbols {

public:
    /**
     * Locate and parse debug info for an image (eg. .dbg or .lbl files).
     *
     * @param imagePath Path to binary being added to memory.
     */
    void add(const std::string &imagePath);
};


#endif //EMULATOR_DEBUGSYMBOLS_H
