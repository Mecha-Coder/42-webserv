#include "../../include/Client.hpp"

Client::Client(Server &_server_)
: _server(_server_), _route(NULL), _contentLen(0)
, _byteSent(0), _keepAlive(false) {}

/////////////////////////////////////////////////////////////////////

void Client::reuseFd()
{
    _route = NULL;

    _data.clear();
    _header.clear();

    _version.clear();
    _contentType.clear();
    _contentLen = 0;

    _reply.clear();
    _byteSent = 0;

    _method.clear();
    _uri.clear();
    _path.clear();
    _file.clear();
    _host.clear();
    _keepAlive = false;
}

/////////////////////////////////////////////////////////////////////

void Client::showData()
{
    std::cout << "Client data"
              << "\n============\n\n"
              << "Body\n"
              << "******\n[" << _data << "]\n\n" 
              << "Header: " << std::endl;
    
    Header::const_iterator i = _header.begin();
    for (; i != _header.end(); i++)
        std::cout << "\t[" << i->first << "] → [" << i->second << "]" << std::endl;

    std::cout << "Route: " << (_route? "Available\n\n" :  "NULL\n\n")
              << "Bytes Sent = " << _byteSent << "\n\n"
              << "Others\n" 
              << "*******"
              << "\tMethod      : [" << _method << "]\n"
              << "\tURI         : [" << _uri << "]\n"
              << "\tPath        : [" << _path << "]\n"
              << "\tFile        : [" << _file << "]\n"
              << "\tVersion     : [" << _version << "]\n"
              << "\tContentType : [" << _contentType << "]\n"
              << "\tContentLen  : [" << _contentLen << "]\n"
              << "\tHost        : [" << _host << "]\n"
              << "\tKeepAlive:  : [" << (_keepAlive? "True\n\n" :  "False\n\n") << std::endl;
}