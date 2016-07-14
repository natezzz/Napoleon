#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include "socket.hpp"

namespace napoleon {

class Server
{
public:
    void serve();

private:
    void serveRequest(Request req);

public:
    Server();
    ~Server();

private:
    Socket socket;
};

} // namespace napoleon

#endif // SERVER_HPP_INCLUDED
