#include "Via.h"
#include "Log.h"

#define LOG_TAG "Via"

Via::Via() {
    state = {};
}

Via::~Via() {

}

void Via::storeByte(const Address &address, uint8_t value) {
    uint8_t reg = address.getOffset() & 0x0F;
    switch(reg) {
        case VIA_PORTB:
            state.outputRegisterB = value;
            Log::dbg(LOG_TAG).str("Port B updated ").hex(value).show();
            break;
        case VIA_PORTA:
        case VIA_PORTA_2:
            state.outputRegisterA = value;
            Log::dbg(LOG_TAG).str("Port A updated ").hex(value).show();
            break;
        case VIA_DDRB:
            state.dataDirectionRegisterB = value;
            break;
        case VIA_DDRA:
            state.dataDirectionRegisterA = value;
            break;
        case VIA_T1C_L:
            state.timer1Counter = (state.timer1Counter & 0xFF00) | value;
            break;
        case VIA_T1C_H:
            state.timer1Counter = (state.timer1Counter & 0x00FF) | (value << 8);
            startTimer1();
            break;
        case VIA_T1L_L:
            state.timer1Latch = (state.timer1Latch & 0xFF00) | value;
            break;
        case VIA_T1L_H:
            state.timer1Latch = (state.timer1Latch & 0x00FF) | (value << 8);
            break;
        case VIA_T2C_L:
            state.timer2Counter = (state.timer2Counter & 0xFF00) | value;
            break;
        case VIA_T2C_H:
            state.timer2Counter = (state.timer2Counter & 0x00FF) | (value << 8);
            break;
        case VIA_SR:
            state.shiftRegister = value;
            break;
        case VIA_ACR:
            state.auxiliaryControlRegister = value;
            break;
        case VIA_PCR:
            state.peripheralControlRegister = value;
            break;
        case VIA_IFR:
            state.interruptFlagRegister = value;
            break;
        case VIA_IER:
            state.interruptEnableRegister = value;
            break;
    }
}

uint8_t Via::readByte(const Address &address) {
    uint8_t reg = address.getOffset() & 0x0F;
    // TODO return values..
    return 0;
}

bool Via::decodeAddress(const Address &in, Address &out) {
    out = in;
    return in.getBank() == 0 && (in.getOffset() & 0xFE00) == 0xC000;
}

void Via::clockTick(int i) {
    if(state.isTimer1Running) {
        if(state.timer1Counter < i) {
            // TODO might fire interrupt depending on config
            const int ACR_T1_CONTINUOUS = 0x40;
            if((state.auxiliaryControlRegister & ACR_T1_CONTINUOUS) == ACR_T1_CONTINUOUS) {
                // Reload counter from latch and go again
                state.timer1Counter = state.timer1Latch;
            } else {
                // One-shot mode
                state.timer1Counter = 0;
                state.isTimer1Running = false;
            }
        } else {
            state.timer1Counter -= i;
        }
    }
}

void Via::startTimer1() {
    state.isTimer1Running = true;
    state.timer1Latch = state.timer1Counter;
}
