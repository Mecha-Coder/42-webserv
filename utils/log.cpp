#include "../include/webserv.hpp"

void logError(const Str &where, Str error)
{
    std::cout << RED
              << "[" << where << "] : "
              << error << RESET << std::endl;
}

void logAction(const Str &where, Str action)
{
    std::cout << GREEN
              << "[" << where << "] : "
              << action << RESET << std::endl;
}

void logNote(const Str &where, Str note)
{
    std::cout << CYAN
              << "[" << where << "] : "
              << note << RESET << std::endl;
}
