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

/////////////////////////////////////////////////////////////////////////

Route *Server::findRoute(const Str uri)
{
    Routes::iterator i = _routes.begin();
    for( ; i != _routes.end(); i++)
    {
        if (i->_uri == uri) return &(*i);
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////

void Server::giveMyAddr(IP_Host &addrList) const
{
    size_t pos;
    Str IP, Port;

    Address::const_iterator i = _listen.begin();
    for (; i != _listen.end() ; i++)
    {
        pos  = i->find(":");
        IP   = i->substr(0, pos);
        Port = i->substr(pos + 1);
        addrList.push_back(std::make_pair(IP, Port));
    }
}

/////////////////////////////////////////////////////////////////////////

/*
bool Server::isMyAddr(const Str &host) const
{
    Address::const_iterator i = _listen.begin();
    for (; i != _listen.end() ; i++)
    {
        if (*i == host) return true;
    }
    return false;
}
*/

/////////////////////////////////////////////////////////////////////////

const Str Server::myErrorPg(const int &code) const
{
    ErrorPage::const_iterator i = _errorPage.begin();
    for (; i != _errorPage.end(); i++)
    {
        if (i->first == code)
            return (_mainRoot + i->second);
    }
    return "";
}

/////////////////////////////////////////////////////////////////////////

void Server::showData() const
{
    std::cout << "Server data for " << _serverName << std::endl;
    std::cout << "=====================================\n"
              << "Main Root    : [" << _mainRoot << "]\n"
              << "Cliet Max    : [" << _clientBodySize << "]\n"
              << "Listen       : " << std::endl;
    
    for (size_t i = 0; i < _listen.size(); i++)
        std::cout << "\t" << _listen[i] << std::endl;
    
    std::cout << "\nError Pg : " << std::endl;
    
    ErrorPage::const_iterator j = _errorPage.begin();
    for (; j != this->_errorPage.end(); j++)
        std::cout << "\t" << j->first << ", " << j->second << std::endl;
    std::cout << std::endl;

    for (size_t k = 0; k < _routes.size(); k++)
        _routes[k].showData();
}