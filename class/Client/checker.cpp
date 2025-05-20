#include "../../include/Client.hpp"

/* 
Purpose:
- Check if the body size match the content-length
⚠️ NOTE: Call this ONLY after the request has been parsed and it's a POST method.
_________________________________________________________________________________

Return:
- true  ➡️ Same.
- false ➡️ Not same.
*/
bool Client::isBodyMatchLen() const
{return (this->data.size() == this->_contentLen);}

/* 
Purpose:
- Check if bodySize doesn't exceed client max body size set in config
⚠️ NOTE: Call this ONLY after the request has been parsed and it's a POST method.
_________________________________________________________________________________

Return:
- true  ➡️ Below limit.
- false ➡️ Exceed Max Body Size Limit.
*/
bool Client::isBodyWithinLimit() const
{return (this->data.size() <= this->server._clientBodySize );}

/*
Purpose:
- Check if the request header has already been parsed.
- If so, we can retrieve the desired values.

_______________________________________________________________________________

Return:
    TRUE  ➡️ HTTP header has already been parsed.
    FALSE ➡️ Wait
*/
bool Client::isHeadReady() const
{ return (header.size() && !_method.empty()); }

/* 
Purpose:
- Check if route is available in the config
⚠️ NOTE: Call this ONLY after the request
_________________________________________________________________________________

Return:
- true  ➡️ Found route
- false ➡️ Invalid route respond with 404
*/
bool Client::haveRoute() const
{return (this->route != NULL);}

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
bool   Client::isCGI() const
{ return route? this->route->runWithCGI(this->_file) : false; }

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
bool  Client::isMethodAllow() const
{ return route? this->route->isMethodAllow(this->_method) : false; }

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
- Decide whether connection (clientFd) should be closed after the response 
- Or kept open for reuse.

_______________________________________________________________________________

Return:
- true  ➡️ HTTP Connection header is explicitly set to 'keep-alive'.
- false ➡️ HTTP Connection header is set to 'close' or is missing.
*/
bool  Client::isKeepAlive() const
{   
    Header::const_iterator i = this->header.find("Connection");
    
    if (i != header.end())
    {
        if (i->second == "keep-alive") return true; 
    }
    return false;
}