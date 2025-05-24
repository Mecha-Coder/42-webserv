#include "../../include/Client.hpp"

bool deleteFile(const Str& filePath);

void Client::resDeleteFile()
{
    if (deleteFile(this->_filePath + this->_file))
    {
        Str result = 
            "HTTP/1.1 204 No Content\r\n"
            "Content-Length: 0\r\n\r\n";
        this->reply.insert(this->reply.end(), result.begin(), result.end());

        logMsg(this->getHost() + " | resDeleteFile", "Respond: 204: Successfully deleted " + this->_file, 1);
    }
    else 
    { 
        logMsg(this->getHost() + " | resDeleteFile", "Failed to delete " + this->_file, 0);
        this->resError(500);
    } 
}