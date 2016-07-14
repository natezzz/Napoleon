#ifndef REQUEST_HPP_INCLUDED
#define REQUEST_HPP_INCLUDED

namespace napoleon {

class Request
{
public:
    enum RequestType {
        GET,
        POST,
        UNKNOWN
    };
    inline RequestType getType() { return type; }
    inline void setType(RequestType t) { type = t; }

public:
    Request() : type(UNKNOWN) {}
    ~Request() {};

private:
    RequestType type;
};

} // namespace napoleon

#endif // REQUEST_HPP_INCLUDED
