#include "../../include/Client.hpp"

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