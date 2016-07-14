#ifndef ERROR_HPP_INCLUDED
#define ERROR_HPP_INCLUDED

#include <iostream>
#include <string>
#include <cstdlib>

namespace napoleon {

void die(std::string why)
{
    std::cerr << why << ": " << strerror(errno) << std::endl;
    ::exit(-1);
}

} // namespace napoleon

#endif // ERROR_HPP_INCLUDED
