#include "../../include/Client.hpp"

Str     statusBuilder(int code);
Str     contentBuilder(const Str &filename, size_t size);
bool    readFile(const Str& filename, Binary &content);

void Client::resFectchFile()
{
    Binary content;

    if (readFile(this->_filePath + this->_file, content))
    {
        Str statusLine = statusBuilder(200);
        Str contentLine = contentBuilder(this->_file, content.size());
        
        this->reply.insert(this->reply.end(), statusLine.begin(), statusLine.end());
        this->reply.insert(this->reply.end(), contentLine.begin(), contentLine.end());
        this->reply.push_back('\r');
        this->reply.push_back('\n');
        this->reply.insert(this->reply.end(), content.begin(), content.end());
    }
    else
        this->resError(500);
}