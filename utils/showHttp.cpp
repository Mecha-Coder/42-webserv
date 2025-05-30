#include "../include/webserv.hpp"

/* 
Purpose:
- Used for troubleshooting
- Print out the raw form of http request and respond, even the character "/r/n" 
_______________________________________________________________________________

Return: No return
*/
void showRawStr(const Str &content)
{
    for (size_t i = 0; i < content.size(); i++)
    {
        if (content[i] == '\r')         std::cout << "\\r";
        else if (content[i] == '\n')    std::cout << "\\n\n";
        else if (content[i] == '\t')    std::cout << "\\t";
        else                            std::cout << content[i];
    }
    std::cout << "\n" << std::endl;
}
