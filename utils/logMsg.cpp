#include "../include/webserv.hpp"

/*
TRUE  - OK    - CYAN
FALSE - ERROR - RED

void logMsg(const Str &where, Str action, bool state)
{
    std::cout << (state? CYAN : RED) 
              << "[" << where << "] : "
              << action << RESET << std::endl;
}
*/


void logError(const Str &where, Str error)
{
    std::cout << RED
              << "[" << where << "] : "
              << error << RESET << std::endl;
}

void logAction(const Str &where, Str action)
{
    std::cout << CYAN
              << "[" << where << "] : "
              << action << RESET << std::endl;
}
