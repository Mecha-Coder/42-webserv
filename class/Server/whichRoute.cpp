#include "../../include/Server.hpp"

/*
Purpose:
- Check if the request path (URI) matches any route defined in the server's config.
- If a match is found, return the corresponding Route object to simplify access to route details.
- If no match is found, return NULL.

_______________________________________________________________________________

Return:
- Route object ➡️ Matching path/URI found in the configuration
- NULL         ➡️ No matching path/URI
*/
const Route *Server::findRoute(const Str uri) const
{
    Routes::const_iterator i = _routes.begin();
    for( ; i != _routes.end(); i++)
    {
        if (i->_uri == uri) return &(*i);
    }
    return NULL;
}