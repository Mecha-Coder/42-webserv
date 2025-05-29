#include "../../include/Client.hpp"

const Str &Client::getRedirect() const
{ return _route->_redirect;}

const Str   Client::_myErrorPg(const int &code) const
{ return this->server.myErrorPg(code); }

const Str   Client::getDefaultFile() const
{ return route? this->route->_defaultFile : ""; }

