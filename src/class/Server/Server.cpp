#include "../../include/Server.hpp"

/////////////////////////////////////////////////////////////////////////

Server::Server(
    const Str       &serverName,
    const List      &listen,
    const ErrorPage &errorPage,
    const Routes    &routes,
    const Str       &root,
    const size_t    &clientBody
) : 
    _serverName(serverName), 
    _listen(listen),
    _errorPage(errorPage),
    _routes(routes),
    _root(root), 
    _clientBody(clientBody) 
{}

/////////////////////////////////////////////////////////////////////////

Route *Server::findRoute(const Str &uri)
{
    Routes::iterator i;

    for( i = _routes.begin(); i != _routes.end(); i++)
    {
        if (i->_path.size() > uri.size())
            continue;
        
        if (uri.compare(0, i->_path.size(), i->_path) == 0)
            return &(*i);
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////

void Server::giveListenPort(Ports &portList) const
{
    List::const_iterator i;

    for (i = _listen.begin(); i != _listen.end() ; i++)
        portList.insert(*i);
}

/////////////////////////////////////////////////////////////////////////

bool Server::matchHost(const Str &host) const
{
    List::const_iterator i;
    for (i = _listen.begin(); i != _listen.end(); i++)
    {
        std::cout << "Compare " << (_serverName + ":" + *i) << " against " << host << std::endl;
        if ((_serverName + ":" + *i) == host)
            return true;

        std::cout << "Compare " << (Str(IP) + ":" + *i) << " against " << host << std::endl;
        if ((Str(IP) + ":" + *i) == host)
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////

bool Server::matchPort(const Str &port) const
{
    List::const_iterator i;
    for (i = _listen.begin(); i != _listen.end(); i++)
    {
        std::cout << "Compare " << *i << " against " << port << std::endl;
        if (*i == port)
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////

const Str Server::myErrorPg(const int &code) const
{
    ErrorPage::const_iterator i = _errorPage.begin();

    for (; i != _errorPage.end(); i++)
    {
        if (i->first == code)
            return (_root + i->second);
    }
    return "";
}

/////////////////////////////////////////////////////////////////////////
