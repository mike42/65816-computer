#ifndef EMULATOR_RAM_H
#define EMULATOR_RAM_H

#include <SystemBusDevice.h>
#include <cstddef>
#include <string>

/**
 * Emulate RAM
 */
class Ram : public SystemBusDevice {
private:
    uint8_t *mRam;

public:

    explicit Ram(uint8_t);
    ~Ram() override;

    void storeByte(const Address &, uint8_t) override;
    uint8_t readByte(const Address &) override;
    bool decodeAddress(const Address &, Address &) override;
    void loadFromFile(const std::string& fileName, const Address& start, size_t size);
};

#endif //EMULATOR_RAM_H
