#ifndef EMULATOR_UART_H
#define EMULATOR_UART_H

#include "SystemBusDevice.h"
#include "TerminalWrapper.h"

struct UartState {
    uint8_t lineControlRegister;
    unsigned char charPending;
    bool hasCharPending;
};

/**
 * Emulate just some features of a NXP SC16C752
 */
class Uart : public SystemBusDevice {
public:
    // General register set
    static const int UART_RHR = 0x00; // Receiver Holding Register (RHR) - R
    static const int UART_THR = 0x00; // Transmit Holding Register (THR) - W
    static const int UART_IER = 0x01; // Interrupt Enable Register (IER) - R/W
    static const int UART_IIR = 0x02; // Interrupt Identification Register (IIR) - R
    static const int UART_FCR = 0x02; // FIFO Control Register (FCR) - W
    static const int UART_LCR = 0x03; // Line Control Register (LCR) - R/W
    static const int UART_MCR = 0x04; // Modem Control Register (MCR) - R/W
    static const int UART_LSR = 0x05; // Line Status Register (LSR) - R
    static const int UART_MSR = 0x06; // Modem Status Register (MSR) - R
    static const int UART_SPR = 0x07; // Scratchpad Register (SPR) - R/W
    // Special register set
    static const int UART_DLL = 0x00; // Divisor latch LSB - R/W
    static const int UART_DLM = 0x01; // Divisor latch MSB - R/W
    // Enhanced register set;
    static const int UART_EFR = 0x02; // Enhanced Feature Register (EFR) - R/W
    static const int UART_TCR = 0x06; // Transmission Control Register (TCR) - R/W
    static const int UART_TLR = 0x07; // Trigger Level Register (TLR) - R/W

    Uart(TerminalWrapper *terminal);

    void storeByte(const Address &address, uint8_t uint8) override;

    uint8_t readByte(const Address &address) override;

    bool decodeAddress(const Address &address, Address &address1) override;

private:
    TerminalWrapper *terminal;
    UartState state;

    void tryRead();
};


#endif //EMULATOR_UART_H
