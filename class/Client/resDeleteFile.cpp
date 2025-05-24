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
        std::cout << "Successfully deleted " << this->_file << std::endl;
    }
    else { 
        std::cout << "Failed to delete " << this->_file << std::endl;
        this->resError(500);
    } 
}