#include "../../include/Client.hpp"

/* 
Purpose:
- Check if the body size match the content-length
⚠️ NOTE: Call this ONLY after the request has been parsed and it's a POST method.
_________________________________________________________________________________

Return:
- true  ➡️ Same.
- false ➡️ Below limit.
*/
bool Client::bodySizeMatch() const
{return (this->data.size() == this->_contentLen);}