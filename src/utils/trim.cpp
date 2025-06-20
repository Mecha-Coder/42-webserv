#include "../include/webserv.hpp"

Str trim(const Str s) 
{
    const Str whitespace = " \t\n\r\f\v";

    size_t start = s.find_first_not_of(whitespace);
    if (start == std::string::npos)
        return ""; // string is all whitespace

    size_t end = s.find_last_not_of(whitespace);
    return s.substr(start, end - start + 1);
}