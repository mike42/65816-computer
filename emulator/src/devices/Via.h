#ifndef EMULATOR_VIA_H
#define EMULATOR_VIA_H

#include "SystemBusDevice.h"

/**
 * State of the VIA
 */
struct ViaState {
    uint8_t outputRegisterB;
    uint8_t inputRegisterB;
    uint8_t inputRegisterA;
    uint8_t outputRegisterA;
    uint8_t dataDirectionRegisterA;
    uint8_t dataDirectionRegisterB;
    uint16_t timer1Counter;
    uint16_t timer1Latch;
    uint16_t timer2Counter;
    uint8_t shiftRegister;
    uint8_t auxiliaryControlRegister;
    uint8_t peripheralControlRegister;
    uint8_t interruptFlagRegister;
    uint8_t interruptEnableRegister;
    bool isTimer1Running;
};

/**
 * Emulate some features of 65C22 Versatile Interface Adapter (VIA)
 */
class Via : public SystemBusDevice {
public:
    // Register ID's as accessed over data bus
    static const int VIA_PORTB = 0x00;
    static const int VIA_PORTA = 0x01;
    static const int VIA_DDRB = 0x02;
    static const int VIA_DDRA = 0x03;
    static const int VIA_T1C_L= 0x04;
    static const int VIA_T1C_H = 0x05;
    static const int VIA_T1L_L = 0x06;
    static const int VIA_T1L_H = 0x07;
    static const int VIA_T2C_L = 0x08;
    static const int VIA_T2C_H = 0x09;
    static const int VIA_SR = 0x0A;
    static const int VIA_ACR = 0x0B;
    static const int VIA_PCR = 0x0C;
    static const int VIA_IFR = 0x0D;
    static const int VIA_IER = 0x0E;
    static const int VIA_PORTA_2 = 0x0F;

    explicit Via();
    ~Via() override;

    void storeByte(const Address &, uint8_t) override;
    uint8_t readByte(const Address &) override;
    bool decodeAddress(const Address &, Address &) override;
    void clockTick(int ticks) override;
private:
    void startTimer1();

    ViaState state;
};


#endif //EMULATOR_VIA_H
