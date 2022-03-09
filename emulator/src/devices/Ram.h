#ifndef EMULATOR_RAM_H
#define EMULATOR_RAM_H

#include <SystemBusDevice.h>

class Ram : public SystemBusDevice {
private:
    uint8_t *mRam;

public:

    Ram(uint8_t);
    ~Ram();

    void storeByte(const Address &, uint8_t);
    uint8_t readByte(const Address &);
    bool decodeAddress(const Address &, Address &);
};

#endif //EMULATOR_RAM_H
