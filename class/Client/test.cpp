#include "../../include/Client.hpp"

int main()
{
    Client dummy;

    if (dummy.appendReq("GET / HTTP/1.1\r\nHost: example."))
        std::cout << "Found end of headers\n";
    else
        std::cout << "Not found\n";


     if (dummy.appendReq(".com\r\n\r\nSome body"))
        std::cout << "Found end of headers\n";
    else
        std::cout << "Not found\n";

    dummy.showDetails();
}