#include "../../include/Client.hpp"

void Client::resRedirectTo()
{
    Str result = 
            "HTTP/1.1 301 Moved Permanently\r\n"
            "Location: " + this->_redirect + "\r\n"
            "Content-Length: 0\r\n\r\n";

    this->reply.insert(this->reply.end(), result.begin(), result.end());
}
