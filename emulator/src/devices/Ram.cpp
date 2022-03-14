#include "Ram.h"

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