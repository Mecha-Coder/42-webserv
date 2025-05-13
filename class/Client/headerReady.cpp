#include "../../include/Client.hpp"

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