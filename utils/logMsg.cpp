#include "../include/webserv.hpp"

/*
TRUE  - OK    - CYAN
FALSE - ERROR - RED
*/
void logMsg(const Str &where, Str action, int state)
{
    std::cout << (state? CYAN : RED) 
              << "[" << where << "] : "
              << action << RESET << std::endl;
}

