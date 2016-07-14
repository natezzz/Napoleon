#include <iostream>

#include "defines.hpp"
#include "socket.hpp"
#include "server.hpp"
#include "logger.hpp"

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
            Logger::logInfo("Method: GET");
            break;
        case Request::RequestType::POST:
            Logger::logInfo("Method: POST");
            break;
        default:
            Logger::logError("Method: Unknown");
            break;
    }
}

} // namespace napoleon
