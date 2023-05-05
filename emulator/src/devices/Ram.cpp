#include <iostream>
#include <cassert>
#include "Ram.h"
#include "Log.h"

#define LOG_TAG "Ram"

Ram::Ram(uint8_t banks) {
    mRam = new uint8_t[banks * BANK_SIZE_BYTES];
    mRamSize = banks * BANK_SIZE_BYTES;
}

Ram::~Ram() {
    delete[] mRam;
}

void Ram::storeByte(const Address &address, uint8_t value) {
    // Note this wraps if you go outside usable RAM
    size_t offset = address.getAbsolute() % mRamSize;
    mRam[offset] = value;
}

uint8_t Ram::readByte(const Address &address) {
    size_t offset = address.getAbsolute() % mRamSize;
    return mRam[offset];
}

bool Ram::decodeAddress(const Address &in, Address &out) {
    out = in;
    return true;
}

void Ram::loadFromFile(const std::string& fileName, const Address &start, const size_t size) {
    FILE *fp = fopen(fileName.c_str(), "rb");
    assert(fp != nullptr);
    assert(start.getAbsolute() + size <= mRamSize); // Hopefully keep us within the boundary.
    size_t bytesRead = fread(mRam + start.getAbsolute(), 1, size, fp);
    fclose(fp);
    assert(bytesRead == size);
    Log::err(LOG_TAG).str("Ram initialised from ").str(fileName.c_str()).show();
}
