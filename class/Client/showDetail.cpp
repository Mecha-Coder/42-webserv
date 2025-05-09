#include "../../include/Client.hpp"

void Client::showDetails()
{
    std::cout << "data\n-------\n" << data << "\n\n"
              << "header\n--------\n";
    
    Header::iterator i = header.begin();
    for (; i != header.end(); i++)
        std::cout << i->first << " : " << i->second << "\n";
    std::cout << "\n\n";

    std::cout << "method     : " << method << "\n"
              << "path       : " << path << "\n"
              << "version    : " << version << "\n"
              << "host       : " << host << "\n"
              << "file       : " << file << "\n"
              << "contentType: " << contentType << "\n"
              << "contentLen : " << contentLen << "\n"
              << "keepAlive  : " << keepAlive << "\n";
}
