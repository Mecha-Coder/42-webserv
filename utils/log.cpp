#include "../include/webserv.hpp"

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
