//
// Created by Francesco Rigoni on 05/03/2018.
//

#ifndef LIB65816_SAMPLE_PROGRAMRAM_H
#define LIB65816_SAMPLE_PROGRAMRAM_H

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


#endif //LIB65816_SAMPLE_PROGRAMRAM_H
