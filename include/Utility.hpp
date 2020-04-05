#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iomanip>
#include <sstream>

template<class T>
static std::string hex(T i)
{
    std::stringstream ss;
    ss << std::setfill('0')
       << std::setw(sizeof(T) * 1)
       << std::hex << i;

    return ss.str();
}

#endif // UTILITY_HPP
