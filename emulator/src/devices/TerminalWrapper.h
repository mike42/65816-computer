#ifndef EMULATOR_TERMINALWRAPPER_H
#define EMULATOR_TERMINALWRAPPER_H


class TerminalWrapper {
public:
    TerminalWrapper();

    void write(char charToWrite);

    char read();

    virtual ~TerminalWrapper();

};


#endif //EMULATOR_TERMINALWRAPPER_H
