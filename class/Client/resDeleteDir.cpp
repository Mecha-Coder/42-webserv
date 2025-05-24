#include "../../include/Client.hpp"

bool deleteFile(const Str& filePath);
bool deleteFolder(const Str& path);

void Client::resDeleteDir()
{
    if (deleteFolder(this->_filePath))
    {
        Str result = 
            "HTTP/1.1 204 No Content\r\n"
            "Content-Length: 0\r\n\r\n";
        this->reply.insert(this->reply.end(), result.begin(), result.end());

        logMsg(this->getHost() + " | resDeleteDir", "Respond: 204: Successfully deleted folder" + this->_path, 1);
    }
    else 
    { 
        logMsg(this->getHost() + " | resDeleteDir", "Failed to delete folder " + this->_path, 0);
        this->resError(500);
    } 
}
