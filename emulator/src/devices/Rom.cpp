#include <cassert>
#include "Rom.h"

#define LOG_TAG "Rom"

#include "Log.h"

Rom::~Rom() = default;

Rom::Rom(const std::string &file_name) {
    FILE *fp = fopen(file_name.c_str(), "rb");
    assert(fp != nullptr);
    size_t bytes_read = fread(this->rom, 1, ROM_SIZE, fp);
    fclose(fp);
    assert(bytes_read == ROM_SIZE);
    Log::err(LOG_TAG).str("Rom initialised from ").str(file_name.c_str()).show();
}

void Rom::storeByte(const Address &address, uint8_t value) {
    // Do absolutely nothing
}

uint8_t Rom::readByte(const Address &address) {
    return this->rom[address.getOffset() % ROM_SIZE];
}

bool Rom::decodeAddress(const Address &in, Address &out) {
    out = in;
    return in.getBank() == 0 && (in.getOffset() & 0xE000) == 0xE000;
}

uint16_t Rom::vector_at(uint16_t offset) {
    return this->readByte(Address(0x00, offset)) + (this->readByte(Address(0x00, offset + 1)) << 8);
}
