#include "../../include/Client.hpp"

Client::Client()
: _server(NULL), _route(NULL), _contentLen(0)
, _byteSent(0), _keepAlive(false) {}

/////////////////////////////////////////////////////////////////////

bool Client::trackReply(size_t count)
{
    _byteSent += count;
    if (_reply.size() > _byteSent)
        return false;
    if (_reply.size() < _byteSent)
        logError("outgoing | trackReply", "_byteSend= " + toStr(_byteSent) + " is incorrect");
    return true;
}

/////////////////////////////////////////////////////////////////////

const char *Client::getReply(size_t &remainSize)
{
    if (_reply.size() - _byteSent <= 0)
    {
        logError("outgoing | getReply", "_byteSend= " + toStr(_byteSent) + " is incorrect");
        return &_reply[0];
    }

    remainSize = _reply.size() - _byteSent;
    return &_reply[_byteSent];
}

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
    _host.clear();
    _fullPath.clear();
    _keepAlive = false;
}

/////////////////////////////////////////////////////////////////////
