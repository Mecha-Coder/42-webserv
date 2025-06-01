#include "../include/webserv.hpp"

size_t strToSizeT(const Str &s)
{
    std::stringstream ss(s);
    size_t result = 0;
    ss >> result;

    if (ss.fail() || !ss.eof())
        return 0;
    return result;
}