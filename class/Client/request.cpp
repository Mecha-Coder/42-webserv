#include "../../include/Client.hpp"

size_t strToSizeT(const Str &s);

///////////////////////////////////////////////////////////////////////////////////

void Client::parseLine(Str line)
{
    size_t pos, slash, dot;

    for (int i = 0; (pos = line.find(" ")) != line.npos; i++)
    {
        if (i == 0)
            _method = line.substr(0, pos);
        if (i == 1)
        {
            _uri = line.substr(0, pos);

            slash = _uri.rfind("/");
            dot   = _uri.rfind(".");

            if (dot != _uri.npos && dot > slash)
            {
                _file = _uri.substr(slash + 1);
                _path = _uri.substr(0, slash + 1);
            }
            else _path = _uri;
        }
        line.erase(0, pos + 1);
    }
    _version = line;
}

///////////////////////////////////////////////////////////////////////////////////

void Client::parseHead(Str info)
{
    Str key, value;
    size_t pos = info.find(": ");

    if (pos != info.npos)
    {
        key = info.substr(0, pos);
        value = info.substr(pos + 2);
        _header[key] = value;

        if (key == "Content-Length")  _contentLen = strToSizeT(value);
        if (key == "Content-Type")    _contentType = value;
        if (key == "Host")            _host = value;
        if (key == "Connection" && value == "keep-alive") _keepAlive = true;
    }
}

///////////////////////////////////////////////////////////////////////////////////

bool Client::appendReq(const char *request, size_t byteRead)
{
    Str     info;
    size_t  pos;
    
    _data.append(request, byteRead);
    
    if (_header.empty() && _method.empty())
    {
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
         _route = _server.findRoute(_path);
    }
    return (_data.size() >= _contentLen);
}


