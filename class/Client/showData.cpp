#include "../../include/Client.hpp"

void Client::showData()
{
    std::cout << "Request data:\n===========\n" << data << "\n\n"
              << "Header data: " << std::endl;
    
    Header::const_iterator i = header.begin();
    for (; i != header.end(); i++)
        std::cout << "\t" << i->first << " → " << i->second << std::endl;
    
    std::cout << "Route: " << (route? "Available\n\n" :  "NULL\n\n")
              << "Respond data:\n===========\n" << reply << "\n"
              << "Bytes Sent = " << byteSend << "\n\n"

              << "Parse Data \n===========\n"
              << "\tMethod → "       << _method      << "\n"
              << "\tPath → "         << _path        << "\n"
              << "\tVersion → "      << _version     << "\n"
              << "\tContent type → " << _contentType << "\n"
              << "\tContent len → "  << _contentLen  << "\n"
              << "\tFile → "         << _file        << "\n"
              << "\tRedirect → "     << _redirect    << "\n"
              << "\tFile path → "    << _filePath    << "\n"
              << "\tUpload dir → "   << _uploadDir   << "\n"
              << "\tCGI path → "     << _cgiPath     << "\n\n"

             << "Getter \n===========\n"
             << "isKeepAlive → " << (this->isKeepAlive()? "Yes":"No") << "\n"
             << "getHost → "     << this->getHost() << std::endl;
}
