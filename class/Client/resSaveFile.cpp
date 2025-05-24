#include "../../include/Client.hpp"

bool saveFile(const Str &filename, const Str &data);

bool processFile(Str &File, Str &saveHere, Str &name)
{
    Str raw  = File.substr(File.find("\r\n\r\n") + 4);
    name = File.substr(0, File.find("\"\r\n"));
    name.erase(0, File.find("filename=\"") + 10);

    return saveFile(saveHere + name, raw);
}

void Client::resSaveFile()
{
    Str File, name;
    Str boundary = this->_contentType;
    boundary = "--" + boundary.substr(boundary.find("boundary=") + 9);

    this->data.erase(0, boundary.size() + 2);
    
    size_t pos = this->data.find(boundary);
    while (pos != this->data.npos)
    {
        File = this->data.substr(0, pos - 2);
        
        if (processFile(File, this->_uploadDir, name))
            logMsg(this->getHost() + " | resSaveFile", name + " saved", 1);
        else
            logMsg(this->getHost() + " | resSaveFile", name + " failed to save", 0);

        this->data.erase(0, pos + boundary.size() + 2);
        pos = this->data.find(boundary);
    }
}
