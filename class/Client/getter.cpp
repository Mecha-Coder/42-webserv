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
- Wrapper function for Server Client Max Body Size

_______________________________________________________________________________

Return:
- Number range from 1 - 2147483647 bytes
*/
int   Client::getMaxBodySize() const
{ return this->server._clientBodySize; }

/*
⚠️ NOTE: Call this ONLY after the request has been parsed.

Purpose:
- Decide whether connection (clientFd) should be closed after the response 
- Or kept open for reuse.

_______________________________________________________________________________

Return:
- true  ➡️ HTTP Connection header is explicitly set to 'keep-alive'.
- false ➡️ HTTP Connection header is set to 'close' or is missing.
*/
bool  Client::iskeepAlive() const
{   
    Header::const_iterator i = this->header.find("Connection");
    
    if (i != header.end())
    {
        if (i->second == "keep-alive") return true; 
    }
    return false;
}     

/*
⚠️ NOTE: Call this ONLY after the request has been parsed.

Purpose:
- Wrapper for Route AutoIndex
- For the current Route, check if AutoIndex is turn ON,

_______________________________________________________________________________

Return:
- true  ➡️ AutoIndex ON
- false ➡️ AutoIndex OFF
*/
bool  Client::isAutoIndex() const
{ return route? this->route->_autoIndex : false; }

/*
⚠️ NOTE: Call this ONLY after the request has been parsed.

Purpose:
- Wrapper for Route List of Allowed Methods
- Return true if method for the current route is allowed

_______________________________________________________________________________

Return:
- true  ➡️ Allowed
- false ➡️ Not allowed
*/
bool  Client::isMethodAllow(const Str &method) const
{ return route? this->route->isMethodAllow(method) : false; }

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

/*
⚠️ NOTE: Call this ONLY after the request has been parsed.

Purpose:
- Wrapper function for Route Run CGI
- Checks if the file for the current route should be handled via CGI.
- If so, returns the absolute path to the CGI binary (root + CGI subdirectory).
- This method will slit the file extension from the filename

_______________________________________________________________________________

Return:
- Absolute path string ➡️ Handled with CGI
- Empty string         ➡️ Not handled with CGI
*/
const Str   Client::runWithCGI(const Str &file) const
{ return route? this->route->runWithCGI(file) : ""; }
