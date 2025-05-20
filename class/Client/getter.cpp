#include "../../include/Client.hpp"

/*
Purpose:
- Wrapper function for Server Error Page
- Determine if server have its own error page for the given status code
- If yes, return the absolute path to that error page (root + subdir)

_______________________________________________________________________________

Return:
- Absolute path string ➡️ Custom error page exists for the code
- Empty string         ➡️ No custom error page defined for the code
*/
const Str   Client::_myErrorPg(const int &code) const
{ return this->server.myErrorPg(code); }


/*
Purpose:
- (Optional use) Check if the 'Host' header exists in the request.
- If found, return the host address.

_______________________________________________________________________________

Return:
- true  ➡️ 'Host' HTTP header is present.
- false ➡️ 'Host' HTTP header is missing.
*/
const Str   Client::getHost() const
{ 
    if (header.find("Host") != header.end())
        return header.at("Host");
    return "";
}

/*
⚠️ NOTE: Call this ONLY after the request has been parsed.

Purpose:
- Wrapper for Route Default File 
_______________________________________________________________________________

Return:
- File Name    ➡️ Default file is configured.
- Empty string ➡️ No default file is set.
*/
const Str   Client::getDefaultFile() const
{ return route? this->route->_defaultFile : ""; }

