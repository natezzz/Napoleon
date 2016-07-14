#ifndef LOGGER_HPP_INCLUDED
#define LOGGER_HPP_INCLUDED

#include <iostream>
#include <string>

namespace napoleon {

class Logger
{
public:
    static inline void logInfo(std::string info) {
        std::cout << "[INFO]: " << info << std::endl;
    }

    static inline void logError(std::string error) {
        std::cerr << "[ERROR]: " << error << std::endl;
    }
};

}

#endif // LOGGER_HPP_INCLUDED
