#include <iostream>
#include "Rom.h"

#define LOG_TAG "Rom"
#include "Log.h"

Rom::~Rom() = default;

Rom::Rom(const std::string& filename) {
    Log::err(LOG_TAG).str("Rom initialised but not implemented: ").str(filename.c_str()).show();
}

void Rom::storeByte(const Address &address, uint8_t value) {
    // Do absolutely nothing
}

uint8_t Rom::readByte(const Address &address) {
    // Read?
    return 0;
}

bool Rom::decodeAddress(const Address &in, Address &out) {
    // Not implemented
    return false;
}
