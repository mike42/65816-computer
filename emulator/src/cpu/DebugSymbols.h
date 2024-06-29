#ifndef EMULATOR_DEBUGSYMBOLS_H
#define EMULATOR_DEBUGSYMBOLS_H

#include <boost/filesystem/path.hpp>
#include <map>
#include <string>
#include "SystemBusDevice.h"

class DebugSymbols {

public:
    DebugSymbols();

    /**
     * Locate and parse debug info for an image (eg. .dbg or .lbl files).
     *
     * @param imagePath Path to binary being added to memory.
     */
    void loadSymbolsForBinary(const boost::filesystem::path &imagePath);

    /**
     * Parse string containing labels - VICE format
     **/
    void loadLabelFile(const std::string &content);

    std::map<std::string, Address> labels;

    std::map<uint32_t, std::string> labelsReverse;
private:
    void loadLabelLine(const std::string &line);
};


#endif //EMULATOR_DEBUGSYMBOLS_H
