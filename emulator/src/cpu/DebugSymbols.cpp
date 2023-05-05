#include "DebugSymbols.h"

#include "boost/filesystem.hpp"
#include <boost/tokenizer.hpp>
#include <string>
#include "Log.h"

#define LOG_TAG "DebugSymbols"

void DebugSymbols::loadSymbolsForBinary(const boost::filesystem::path &imagePath) {
    boost::filesystem::path p = imagePath; // copy because path.replace_extension() is not const for whatever reason.
    boost::filesystem::path debugPath = p.replace_extension(".dbg");
    if (boost::filesystem::exists(debugPath)) {
        Log::dbg(LOG_TAG).str("Found debug symbols, loading this format is not implemented: ").str(
                debugPath.c_str()).show();
    }
    boost::filesystem::path labelPath = p.replace_extension(".lbl");
    if (boost::filesystem::exists(labelPath)) {
        Log::vrb(LOG_TAG).str("Loading labels: ").str(labelPath.c_str()).show();
        std::string content;
        boost::filesystem::load_string_file(labelPath, content);
        DebugSymbols::loadLabelFile(content);
    }
}

void DebugSymbols::loadLabelLine(const std::string &line) {
    // input format: "al 00E001 .some_label"
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    boost::char_separator<char> sep{" "};
    tokenizer tok{line, sep};
    // iterate space-separated tokens within the line
    int i = 0;
    std::string name;
    std::string addrString;
    for (const auto &t: tok) {
        if (i == 0) {
            if (t != "al") {
                return; // only care about 'al' (add label) commands.
            }
        } else if (i == 1) {
            addrString = t;
        } else if (i == 2) {
            if(t.starts_with('.')) {
                // Trim expected leading '.'
                name = t.substr(1);
            } else {
                name = t;
            }
        } else {
            // Too many fields
            return;
        }
        i++;
    }
    if (i != 3) {
        // Not enough fields.
        return;
    }
    // Parse out the address
    unsigned long addrLong;
    try {
        addrLong = std::stoul(addrString, nullptr, 16);
    } catch (std::exception &e) {
        // Out of range or not valid hex address
        return;
    }
    auto address = Address(addrLong & 0xFF0000 >> 16, addrLong & 0xFFFF);
    labels.insert({name, address});
    labelsReverse.insert({addrLong & 0xFFFFFF, name});
}

void DebugSymbols::loadLabelFile(const std::string &content) {
    std::istringstream stream(content);
    std::string line;
    // Iterate lines
    while (std::getline(stream, line)) {
        loadLabelLine(line);
    }
}

DebugSymbols::DebugSymbols() : labels() {

}
