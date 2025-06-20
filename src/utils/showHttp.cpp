#include "../include/webserv.hpp"

void showHttp(const Str &content)
{
    for (size_t i = 0; i < content.size(); i++)
    {
        if (content[i] == '\r')         std::cout << "\\r";
        else if (content[i] == '\n')    std::cout << "\\n\n";
        else                            std::cout << content[i];
    }
    std::cout << "\n" << std::endl;
}
