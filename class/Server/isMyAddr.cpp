#include "../../include/Server.hpp"

/*
Purpose:
- Determine if an incoming client request is intended for this server.
- Compares the HTTP "Host" header with the server's listening address.

_______________________________________________________________________________

Return:
- TRUE  ➡️ Host matches this server
- FALSE ➡️ Host does not match (may belong to a different server)
*/
bool Server::isMyAddr(const Str &host) const
{
    Address::const_iterator i = _listen.begin();
    for (; i != _listen.end() ; i++)
    {
        if (*i == host) return true;
    }
    return false;
}
