#ifndef SOCKET_HPP_INCLUDED
#define SOCKET_HPP_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>

#include <cstdint>

#include "request.hpp"
#include "defines.hpp"

namespace napoleon {

class Socket
{
public:
    Request getRequest();
    inline void setPortNumber(uint16_t port) { this->port = port; }

private:
    Request makeRequest(int fd);
    void init();

public:
    Socket() : port(DEFAULT_PORT_NUMBER) { init(); }
    ~Socket();

private:
    int sockfd;  // socket file descriptor
    uint16_t port;
};

} // namespace napoleon

#endif // SOCKET_HPP_INCLUDED
