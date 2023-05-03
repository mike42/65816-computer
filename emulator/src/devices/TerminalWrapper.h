#ifndef EMULATOR_TERMINALWRAPPER_H
#define EMULATOR_TERMINALWRAPPER_H


class TerminalWrapper {
public:
    TerminalWrapper(bool skip);

    void writeChar(unsigned char charToWrite) const;

    bool readChar(unsigned char &res);

    ~TerminalWrapper();

private:
    int sockfd;
    int connfd;
};


#endif //EMULATOR_TERMINALWRAPPER_H
