#include "../../include/Client.hpp"

void parse_line(Client &client, Str s)
{
    size_t pos;
    
    pos = s.find(' ');
    if (pos != s.npos) client.method = s.substr(0, pos);
    s.erase(0, pos + 1);

    pos = s.find(' ');
    if (pos != s.npos) client.path = s.substr(0, pos);
    s.erase(0, pos + 1);

    client.version = s;
}

void parse_header(Client &client, Str s)
{
    (void)client;
    (void)s;
}

bool Client::appendReq(Str request)
{
    size_t pos;
    this->data += request;

    if (data.find("\r\n\r\n") != std::string::npos)
    {
        pos = data.find("\r\n");
        parse_line(*this ,data.substr(0, pos));
        data.erase(0, pos + 2);

        pos = data.find("\r\n\r\n");
        if (pos != data.npos)
        {
            parse_header(*this, data.substr(0, pos + 4));
            data.erase(0, pos + 4);
        }
        return true;
    }
    return false;
}
