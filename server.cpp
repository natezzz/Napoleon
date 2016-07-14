#include <iostream>

#include "defines.hpp"
#include "socket.hpp"
#include "server.hpp"

namespace napoleon {

Server::Server()
{
    socket.setPortNumber(PORT_NUMBER);
}

Server::~Server() {}

void Server::serve()
{
    for (;;) {
        Request req = socket.getRequest();
        serveRequest(req);
    }
}

void Server::serveRequest(Request req)
{
    switch (req.getType()) {
        case Request::RequestType::GET:
            std::cout << "Method: GET" << std::endl;
            break;
        case Request::RequestType::POST:
            std::cout << "Method: POST" << std::endl;
            break;
        default:
            std::cout << "Method: Unknown" << std::endl;
            break;
    }
}

} // namespace napoleon
