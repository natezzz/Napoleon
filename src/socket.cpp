#include <unistd.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

#include "socket.hpp"
#include "error.hpp"
#include "defines.hpp"

namespace napoleon {

Socket::~Socket() {}

void Socket::init()
{
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        die("sockfd");
    }

    struct sockaddr_in myAddr;
    myAddr.sin_family = AF_INET;
    myAddr.sin_port   = htons(port);
    myAddr.sin_addr.s_addr = INADDR_ANY;
    ::memset(&(myAddr.sin_zero), '\0', 0);

    if (::bind(sockfd, reinterpret_cast<struct sockaddr *>(&myAddr), sizeof(struct sockaddr)) == -1) {
        die("bind");
    }

    if (::listen(sockfd, BACK_LOG) == -1) {
        die("listen");
    }
}


Request Socket::getRequest()
{
    struct sockaddr_in incomingAddr;
    socklen_t sinSize;
    int newFd;

    newFd = ::accept(sockfd, reinterpret_cast<struct sockaddr *>(&incomingAddr), &sinSize);
    if (newFd == -1) {
        die("accept");
    }

    Request req = makeRequest(newFd);
    ::close(newFd);
    return req;
}

Request Socket::makeRequest(int fd)
{
    char buf[MAX_BUF_LEN];
    char method[255];
    int numChars;

    Request req;
    for (;;) {
        numChars = ::recv(fd, buf, MAX_BUF_LEN, 0);
        if (numChars == -1) {
            die("recv");
        }

        if (numChars == 0) {
            std::cout << "recv completed." << std::endl;
            break;
        }

        if (numChars > 0) {
            int i = 0;
            int j = 0;
            while (i < numChars && isspace(buf[i])) {
                i++;  // skip space
            }

            while (i < numChars && !isspace(buf[i])) {
                method[j++] = buf[i++];
            }
            method[j] = '\0';
            break;
        }
    }

    if (strcasecmp(method, "GET") == 0) {
        req.setType(Request::GET);
    } else if (strcasecmp(method, "POST") == 0) {
        req.setType(Request::POST);
    } else {
        req.setType(Request::UNKNOWN);
    }

    return req;
}

} // namespace napoleon
