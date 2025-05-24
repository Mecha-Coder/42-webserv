#include "../../include/Client.hpp"

void Client::resRedirectAddSlash()
{
    Str result = 
            "HTTP/1.1 308 Permanent Redirect\r\n"
            "Location: " + this->_path + "/\r\n"
            "Content-Length: 0\r\n\r\n";

    this->reply.insert(this->reply.end(), result.begin(), result.end());
}