#include "../include/webserv.hpp"

template <typename T>
Str toStr(T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}