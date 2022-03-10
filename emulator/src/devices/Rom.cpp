#include <iostream>
#include <cassert>
#include "Rom.h"

#define LOG_TAG "Rom"

#include "Log.h"

Rom::~Rom() = default;

Rom::Rom(const std::string &file_name) {
    FILE *fp = fopen(file_name.c_str(), "r");
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

NativeModeInterrupts Rom::getNativeModeInterrupts() {
    return {
            .coProcessorEnable = 0x0000,
            .brk = 0x0000,
            .abort = 0x0000,
            .nonMaskableInterrupt = 0x0000,
            .reset = 0x0000,
            .interruptRequest = 0x0000,

    };
}

EmulationModeInterrupts Rom::getEmulationModeInterrupts() {
    return {
            .coProcessorEnable = 0x0000,
            .unused = 0x0000,
            .abort = 0x0000,
            .nonMaskableInterrupt = 0x0000,
            .reset = this->vector_at(0xFFFC),
            .brkIrq = 0x0000
    };
}
