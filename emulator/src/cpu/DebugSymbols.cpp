#include <string>
#include "DebugSymbols.h"

#include "boost/filesystem.hpp"
#include "Log.h"

#define LOG_TAG "DebugSymbols"

void DebugSymbols::add(const std::string& imagePath) {
    boost::filesystem::path p(imagePath);
    boost::filesystem::path debugPath = p.replace_extension(".dbg");
    if(boost::filesystem::exists(debugPath)) {
        Log::vrb(LOG_TAG).str("Found debug symbols, loading this format is not implemented: ").str(debugPath.c_str()).show();
    }
    boost::filesystem::path labelPath = p.replace_extension(".lbl");
    if(boost::filesystem::exists(labelPath)) {
        Log::vrb(LOG_TAG).str("Loading labels: ").str(labelPath.c_str()).show();
    }
}

