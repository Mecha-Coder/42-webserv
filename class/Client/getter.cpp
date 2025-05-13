#include "../../include/Client.hpp"

const int   Client::getMaxBodySize() const
{ return this->server._clientBodySize; }

const bool  Client::iskeepAlive() const
{   
    Header::const_iterator i = header.find("Connection");
    
    if (i != header.end())
    {
        if (i->second == "keep-alive") return true; 
    }
    return false;
}     

/*
Purpose:
- Determine if server have its own error page for the given status code
- If yes, return the absolute path to that error page (root + subdir)

_______________________________________________________________________________

Return:
- Absolute path string ➡️ Custom error page exists for the code
- Empty string         ➡️ No custom error page defined for the code
*/
const Str   Client::_myErrorPg(const int &code) const
{ return this->server.myErrorPg(code); }

const Str   Client::getHost() const
{ 
    if (header.find("Host") != header.end())
        return header.at("Host");
    return "";
}


