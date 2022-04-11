#include <cstdio>
#include "Uart.h"

void Uart::storeByte(const Address &address, uint8_t value) {
    uint8_t reg = address.getOffset() & 0x07;
    // Enhanced register set - available only when LCR is 0xBF
    if (state.lineControlRegister == 0xbf) {
        switch (reg) {
            case UART_EFR:
            case UART_TCR:
            case UART_TLR:
                // Not implemented
                return;
            default:
                // Do nothing
                break;
        }
    }
    // Special register set - available only when top bit of LCR is 1.
    if ((state.lineControlRegister & 0x80) == 0x80) {
        switch (reg) {
            case UART_DLL:
            case UART_DLM:
                // Not implemented
                return;
            default:
                // Do nothing
                break;
        }
    }
    // Regular registers
    switch (reg) {
        case UART_THR:
            terminal->writeChar((char) value);
            return;
        case UART_LCR:
            state.lineControlRegister = value;
            return;
    }
    // If we get here, attempt was made to writeChar to register that is not implemented.
    return;
}

uint8_t Uart::readByte(const Address &address) {
    uint8_t reg = address.getOffset() & 0x07;
    switch (reg) {
        case UART_RHR: {
            tryRead();
            char ret = state.charPending;
            state.charPending = 0x00;
            return ret;
        }
        case UART_LSR:
            tryRead();
            return state.charPending ? 0x01 : 0x00;
    }
    return 0;
}

void Uart::tryRead() {
    if (state.charPending != 0x00) {
        return;
    }
    char res = terminal->readChar();
    state.charPending = res;;
}

bool Uart::decodeAddress(const Address &in, Address &out) {
    out = in;
    return in.getBank() == 0 && (in.getOffset() & 0xFE00) == 0xC200;
}

Uart::Uart(TerminalWrapper *terminal) : terminal(terminal), state({}) {
}
