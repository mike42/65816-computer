#include <termios.h>
#include <unistd.h>
#include "TerminalWrapper.h"

TerminalWrapper::TerminalWrapper() {
    struct termios attrs{};
    tcgetattr(STDIN_FILENO, &attrs);
    attrs.c_lflag &= ~(ECHO); // No local echo, send characters one at a time
    attrs.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &attrs);
}

TerminalWrapper::~TerminalWrapper() {
    struct termios attrs{};
    tcgetattr(STDIN_FILENO, &attrs);
    attrs.c_lflag |= ECHO; // Set terminal back to 'normal'?
    attrs.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &attrs);
}

void TerminalWrapper::writeChar(char ch) {
    write(STDOUT_FILENO, &ch, 1);
}

char TerminalWrapper::readChar() {
    char c;
    if(read(STDIN_FILENO, &c, 1) == 1) {
        return c;
    }
    return 0;
}
