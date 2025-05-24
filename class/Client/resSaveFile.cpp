#include "../../include/Client.hpp"

bool saveFile(const Str &filename, const Str &data);

void processFile(Str &File, Str saveHere)
{
    Str raw  = File.substr(File.find("\r\n\r\n") + 4);
    Str name = File.substr(0, File.find("\"\r\n"));
    name.erase(0, File.find("filename=\"") + 10);

    if (saveFile(saveHere + name, raw))
        std::cout << name << " saved at " << saveHere << std::endl;
    else
        std::cout << name << " failed to save " << std::endl;
    
}

void Client::resSaveFile()
{
    Str File;
    Str boundary = this->_contentType;
    boundary = "--" + boundary.substr(boundary.find("boundary=") + 9);

    this->data.erase(0, boundary.size() + 2);
    
    size_t pos = this->data.find(boundary);
    while (pos != this->data.npos)
    {
        File = this->data.substr(0, pos - 2);
        processFile(File, this->_uploadDir);
        this->data.erase(0, pos + boundary.size() + 2);
        pos = this->data.find(boundary);
    }
}
