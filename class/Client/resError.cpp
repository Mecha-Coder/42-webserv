#include "../../include/Client.hpp"

Str     getCodeShText(int code);
bool    readFile(const Str& filename, Binary &content);

void Client::resError(int code)
{
    Binary body;
    std::ostringstream strCode, strSize;
    Str path = this->_myErrorPg(code);

    if (!path.empty() && readFile(path, body))
    {
        strCode << code;
        strSize << body.size();

        Str head = 
            "HTTP/1.1 " + strCode.str() + " " + getCodeShText(code) + "\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + strSize.str() + "\r\n\r\n";
        
        this->reply.insert(this->reply.end(), head.begin(), head.end());
        this->reply.insert(this->reply.end(), body.begin(), body.end());
    }
    else resDefaultError(code);
}
