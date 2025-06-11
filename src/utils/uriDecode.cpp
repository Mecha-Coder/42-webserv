#include "../include/webserv.hpp"

void uriDecode(Str &s) 
{
    size_t pos;

    while ((pos = s.find("%20")) != s.npos) 
        s.replace(pos, 3, 1, ' ');
}

/*
int main() {
    Str input = "Hello%20World%20from%20C++";
    std::cout << "Before: " << input << std::endl;

    uriDecode(input);

    std::cout << "After:  " << input << std::endl;

    return 0;
}
*/