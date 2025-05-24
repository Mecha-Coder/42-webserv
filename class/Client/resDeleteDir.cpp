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
        std::cout << "Successfully deleted folder " << this->_path << std::endl;
    }
    else { 
        std::cout << "Failed to delete folder " << this->_path << std::endl;
        this->resError(500);
    } 
}
