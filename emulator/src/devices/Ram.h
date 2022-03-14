#ifndef EMULATOR_RAM_H
#define EMULATOR_RAM_H

#include <SystemBusDevice.h>
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
};

#endif //EMULATOR_RAM_H
