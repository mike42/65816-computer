#ifndef EMULATOR_TERMINALWRAPPER_H
#define EMULATOR_TERMINALWRAPPER_H


class TerminalWrapper {
public:
    TerminalWrapper();

    void writeChar(unsigned char charToWrite) const;

    bool readChar(unsigned char &res);

    virtual ~TerminalWrapper();

private:
    int sockfd;
    int connfd;
};


#endif //EMULATOR_TERMINALWRAPPER_H
