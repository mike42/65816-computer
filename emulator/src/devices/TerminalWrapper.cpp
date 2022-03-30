#include <curses.h>
#include "TerminalWrapper.h"

TerminalWrapper::TerminalWrapper() {
    initscr();
    cbreak();
    noecho();
    timeout(0);
}

TerminalWrapper::~TerminalWrapper() {
    endwin();
}

void TerminalWrapper::write(char ch) {
    printw("%c", ch);
    refresh();
}

char TerminalWrapper::read() {
    int intChar = getch();
    if(intChar == -1) {
        return 0;
    }
    // Control characters will not work here..
    return (char)intChar;
}
