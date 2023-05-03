#include <sys/socket.h>
#include <sys/un.h>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "TerminalWrapper.h"

TerminalWrapper::TerminalWrapper(bool skip) {
    if (skip) {
        sockfd = -1;
        connfd = -1;
        return;
    }
    // Adapted from examples at:
    // - https://www.linuxhowtos.org/C_C++/socket.htm
    // - https://www.ibm.com/docs/en/i/7.2?topic=uauaf-example-server-application-that-uses-af-unix-address-family
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw std::runtime_error("couldn't create socket");
    }
    const char *servpath = "serial1";
    unlink(servpath); // Clean up
    struct sockaddr_un serv_addr{};
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, servpath);
    int res = bind(sockfd, (struct sockaddr *) &serv_addr, SUN_LEN(&serv_addr));
    if (res < 0) {
        throw std::runtime_error("failed to bind");
    }
    int rc2 = listen(sockfd, 1);
    if (rc2 < 0) {
        throw std::runtime_error("failed to listen");
    }
    std::cout << "Waiting for client connect to UNIX socket " << servpath << std::endl;
    connfd = accept(sockfd, NULL, NULL);
    if (connfd < 0) {
        throw std::runtime_error("failed to accept");
    }
    // non-blocking reads
    int flags = fcntl(connfd, F_GETFL);
    fcntl(connfd, F_SETFL, flags | O_NONBLOCK);
}

TerminalWrapper::~TerminalWrapper() {
}

void TerminalWrapper::writeChar(const unsigned char ch) const {
    if(sockfd < 0) { // Means not creating unix socket for minicom, pass output to stdout instead.
        std::cout << ch;
        return;
    }
    if (connfd < 0 ) {
        return;
    }
    write(connfd, &ch, 1);
}

bool TerminalWrapper::readChar(unsigned char &res) {
    res = 0x00;
    unsigned char c;
    if (read(connfd, &c, 1) == 1) {
        res = c;
        return true;
    }
    return false;
}

