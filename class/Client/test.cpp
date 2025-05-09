#include "../../include/Client.hpp"

int main()
{
    Client dummy;

    if (dummy.appendReq("POST /login HTTP/1.1\r\nHost: example.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:124.0) Gecko/20100101 Firefox/124.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate, br\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\nOrigin: https:"))
        std::cout << "Found end of headers\n";
    else
        std::cout << "Not found\n";


     if (dummy.appendReq("//example.com\r\nConnection: keep-alive\r\nReferer: https://example.com/login\r\nUpgrade-Insecure-Requests: 1\r\nSec-Fetch-Dest: document\r\nSec-Fetch-Mode: navigate\r\n"))
        std::cout << "Found end of headers\n";
    else
        std::cout << "Not found\n";


    if (dummy.appendReq("Sec-Fetch-Site: same-origin\r\n
Sec-Fetch-User: ?1\r\n
TE: trailers\r\n
\r\n
username=john"))
        std::cout << "Found end of headers\n";
    else
        std::cout << "Not found\n";


        if (dummy.appendReq("&password=1234"))
        std::cout << "Found end of headers\n";
    else
        std::cout << "Not found\n";

    dummy.showDetails();
}