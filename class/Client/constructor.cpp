#include "../../include/Client.hpp"

Client::Client(const Server &_server_)
: server(_server_), route(NULL), byteSend(0), _contentLen(0) {}