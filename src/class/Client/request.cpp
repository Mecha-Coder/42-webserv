#include "../../include/Client.hpp"

size_t strToSizeT(const Str s);
void uriDecode(Str &s);
Str trim(const Str s);

bool not_space(int ch) {
    return !std::isspace(ch);
}

///////////////////////////////////////////////////////////////////////////////////

void Client::parseLine(Str line)
{
    std::istringstream iss(line);
    iss >> _method >> _uri >> _version;

    uriDecode(_uri);
}

///////////////////////////////////////////////////////////////////////////////////

void Client::parseHead(Str info)
{
    Str temp, key, value;
    size_t pos = info.find(":");

    if (pos != info.npos)
    {
        key = trim(info.substr(0, pos));
        value = trim(info.substr(pos + 1));

        _header[key] = value;

        if (key == "Content-Length")
        {
            try { _contentLen = strToSizeT(value); }
            catch(...) {_contentLen = 0;}
        }  
        if (key == "Content-Type")    _contentType = value;
        if (key == "Host")            _host = value;
        if (key == "Connection" && value == "keep-alive") _keepAlive = true;
    }
}

///////////////////////////////////////////////////////////////////////////////////

void Client::getServerData(ServerManager *sManager)
{
    if (_server == NULL)
    {
        _server = sManager->whichServer(_host);
        if (_server == NULL) 
            logError("Parse HTTP header", "Host not matching any server");
    }
    if (_server)_route = _server->findRoute(_uri);
    if (_route)   _fullPath = _server->_root + _uri;
}


///////////////////////////////////////////////////////////////////////////////////

bool Client::appendReq(const char *request, size_t byteRead, ServerManager *sManager)
{
    Str     info;
    size_t  pos;
    
    _data.append(request, byteRead);

    if (_header.empty() && _method.empty())
    {   
        showHttp(_data);
        if ((pos = _data.find("\r\n\r\n")) == std::string::npos)
            return false;
       
        info = _data.substr(0, pos + 2);
        _data.erase(0, pos + 4);
        
        int i = 0;
        while ((pos = info.find("\r\n")) != info.npos)
        {
            if (i == 0)  parseLine(info.substr(0, pos));
            else         parseHead(info.substr(0, pos));

            info.erase(0, pos + 2);
            i++;
        }
        getServerData(sManager);
    }
    return (_data.size() >= _contentLen);
}
