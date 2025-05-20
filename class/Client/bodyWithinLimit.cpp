#include "../../include/Client.hpp"

/* 
Purpose:
- Check if bodySize doesn't exceed client max body size set in config
⚠️ NOTE: Call this ONLY after the request has been parsed and it's a POST method.
_________________________________________________________________________________

Return:
- true  ➡️ Exceed Max Body Size Limit.
- false ➡️ Below limit.
*/
bool Client::bodyWithinLimit() const
{return (this->data.size() <= this->server._clientBodySize );}
