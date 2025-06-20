#include "../include/webserv.hpp"

bool saveFile(const Str &filename, const Str &data)
{
    std::ofstream File(filename.c_str(), std::ios::binary);
    if (!File)
        return false;

    File.write(data.c_str(), static_cast<std::streamsize>(data.size()));
    return File.good();
}

/////////////////////////////////////////////////////////////////////////////////////

bool saveBodyPart(Str &bodyPart, const Str &saveHere, Str &filename)
{
    Str rawData = bodyPart.substr(bodyPart.find("\r\n\r\n") + 4);
    
    filename = bodyPart.substr(0, bodyPart.find("\"\r\n"));
    filename.erase(0, filename.find("filename=\"") + 10);

    return saveFile(saveHere + filename, rawData);
}

///////////////////////////////////////////////////////////////////////////////////

/* OK
int main()
{
    Str bodyPart = 
        "Content-Disposition: form-data; name=\"files\"; filename=\"sampleFile.txt\"\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "types {\r\n"
        "   text/html                              html htm shtml;\r\n"
        "   text/css                               css;\r\n"
        "   text/xml                               xml;\r\n"
        "   image/gif                              gif;\r\n"
        "   image/jpeg                             jpeg jpg;\r\n"
        "   application/javascript                 js;\r\n"
        "   application/atom+xml                   atom;\r\n"
        "   application/rss+xml                    rss;\r\n"
        "}\r\n";
    
    std::cout << bodyPart << std::endl;

    Str filename;
    if (saveBodyPart(bodyPart, "kldsakl/dsadsa/dsa", filename))
        std::cout << "File [" + filename + "] saved" << std::endl;
    else
        std::cout << "Fail to save " + filename << std::endl;
}
*/