#include "../../include/Client.hpp"

Str     getContentType(const Str &filename);
bool    readFile(const Str& filename, Binary &content);

void Client::resFetchFile()
{
    Binary body;
    std::ostringstream oss;

    if (readFile(this->_filePath + this->_file, body))
    {
        oss << body.size();

        Str head = 
            "HTTP/1.1 200 OK \r\n"
            "Content-Disposition: inline; filename=\"" + this->_file + "\"\r\n"
            "Content-Type: " + getContentType(this->_file) + "\r\n"
            "Content-Length: " + oss.str() + "\r\n\r\n";
        
        this->reply.insert(this->reply.end(), head.begin(), head.end());
        this->reply.insert(this->reply.end(), body.begin(), body.end());
        
        logMsg(this->getHost() + " | resFetchFile", "Respond: 200: Successfully read " + this->_file, 1);
    }
    else 
    { 
        logMsg(this->getHost() + " | resFetchFile", "fail to read " + this->_file, 0);
        this->resError(500);
    } 
}
