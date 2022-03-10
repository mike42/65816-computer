#ifndef EMULATOR_ROM_H
#define EMULATOR_ROM_H

#include <string>
#include "SystemBusDevice.h"
#include "Interrupt.h"

#define ROM_SIZE 8192

class Rom : public SystemBusDevice {
private:
    uint8_t rom[ROM_SIZE];

public:
    explicit Rom(const std::string& filename);
    ~Rom() override;

    void storeByte(const Address &, uint8_t) override;
    uint8_t readByte(const Address &) override;
    bool decodeAddress(const Address &, Address &) override;
    EmulationModeInterrupts getEmulationModeInterrupts();
    NativeModeInterrupts getNativeModeInterrupts();
private:
    uint16_t vector_at(uint16_t address);
};
#endif //EMULATOR_ROM_H
