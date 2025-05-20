#include "../../include/Server.hpp"

Server::Server (
    const Str &serverName,
    const Str &mainRoot,
    const size_t &clientBodySize,
    const Address &listen,
    const ErrorPage &errorPage,
    const Routes &routes 
)
: _listen(listen), _errorPage(errorPage), _routes(routes),
_serverName(serverName), _mainRoot(mainRoot), _clientBodySize(clientBodySize) {}
