#include "../../include/Client.hpp"

Client::Client(const Server &_server_)
: _server(_server_), _route(NULL), _byteSent(0), _contentLen(0) {}