#include <iostream>
#include <cassert>
#include "Ram.h"
#include "Log.h"

#define LOG_TAG "Ram"

Ram::Ram(uint8_t banks) {
    mRam = new uint8_t[banks * BANK_SIZE_BYTES];
}

Ram::~Ram() {
    delete[] mRam;
}

void Ram::storeByte(const Address &address, uint8_t value) {
    // TODO this appears to wrap at bank boundaries
    mRam[address.getOffset()] = value;
}

uint8_t Ram::readByte(const Address &address) {
    // TODO this appears to wrap at bank boundaries
    return mRam[address.getOffset()];
}

bool Ram::decodeAddress(const Address &in, Address &out) {
    out = in;
    return true;
}

void Ram::loadFromFile(const std::string& fileName, const Address &start, const size_t size) {
    FILE *fp = fopen(fileName.c_str(), "rb");
    assert(fp != nullptr);
    size_t bytesRead = fread(mRam + start.getOffset(), 1, size, fp);
    fclose(fp);
    assert(bytesRead == size);
    Log::err(LOG_TAG).str("Ram initialised from ").str(fileName.c_str()).show();
}
