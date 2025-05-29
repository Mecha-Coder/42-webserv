#include "../include/webserv.hpp"

bool saveFile(const Str &filename, const Str &data)
{
    std::ofstream File(filename.c_str(), std::ios::binary);
    if (!File)
        return false;

    File.write(data.c_str(), static_cast<std::streamsize>(data.size()));
    return File.good();
}

bool saveMultiPart(Str &bodyPart, const Str &saveHere, Str &filename)
{
    Str rawData = bodyPart.substr(bodyPart.find("\r\n\r\n") + 4);
    
    filename = bodyPart.substr(0, bodyPart.find("\"\r\n"));
    filename.erase(0, filename.find("filename=\"") + 10);

    return saveFile(saveHere + filename, rawData);
}