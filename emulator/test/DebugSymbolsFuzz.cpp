#include "DebugSymbols.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    auto symbols = DebugSymbols();
    auto dataStr = std::string(reinterpret_cast<const char *>(data), size);
    symbols.loadLabelFile(dataStr);
    return 0;
}
