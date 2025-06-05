#include "../include/webserv.hpp"

Str getNow()
{
     std::time_t now = std::time(0);
    std::tm* localTime = std::localtime(&now);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%m/%d/%Y %I:%M:%S %p", localTime);
    return Str(buffer);
}

void logError(const Str &where, Str error)
{
    std::cout << RED "ERROR \t"
              << "[" << getNow() << "] "
              << where << ": "
              << error << RESET << std::endl;
}

void logAction(const Str &where, Str action)
{
    std::cout << GREEN "ACTION\t"
              << "[" << getNow() << "] "
              << where << ": "
              << action << RESET << std::endl;
}

void logNote(const Str &where, Str note)
{
    std::cout << CYAN "NOTE  \t"
              << "[" << getNow() << "] "
              << where << ": "
              << note << RESET << std::endl;
}
