#ifndef EMULATOR_TERMINALWRAPPER_H
#define EMULATOR_TERMINALWRAPPER_H


class TerminalWrapper {
public:
    TerminalWrapper();

    void writeChar(char charToWrite);

    char readChar();

    virtual ~TerminalWrapper();

};


#endif //EMULATOR_TERMINALWRAPPER_H
