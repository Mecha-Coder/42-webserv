#include "../../include/Server.hpp"

Str trim(const Str s);
size_t strToSizeT(const Str s);
int validErrCode(Str key);
template <typename T> Str toStr(T value);

Str printmyList(const List &list);
Str printmyMap(const ErrorPage &map);

//////////////////////////////////////////////////////////////////////////////////////

std::pair<int, Str> getErrPg(Str s)
{
    Str key, value;
    size_t pos = s.find("=");
    if (pos == s.npos)
        throw std::runtime_error("Invalid error page format: Expected <code>=</relative path to root>");
    key = trim(s.substr(0, pos));
    value = trim(s.substr(pos + 1));

    return std::make_pair(validErrCode(key), value);
}

/////////////////////////////////////////////////////////////////////////

bool comparePrefixLen(const LocationConfig& a, const LocationConfig& b) 
{
    return a.prefix.size() > b.prefix.size();
}

/////////////////////////////////////////////////////////////////////////

Server::Server(ServerConfig &data)
{
    size_t i;

    if (data.port.empty())
        throw std::runtime_error("Mandatory data - port missing");
    if (data.host.empty())
        throw std::runtime_error("Mandatory data - host missing");
    if (data.root.empty())
        throw std::runtime_error("Mandatory data - root missing");
    
    _ip = trim(data.host);
    _root = trim(data.root);

    for (i = 0; i < data.port.size(); i++)
        _port.push_back(trim(data.port[i]));

    if (!data.client_body.empty())
        _clientBody = strToSizeT(trim(data.client_body));
    else
        _clientBody = 0;

    if (!data.server_name.empty())
        _serverName = trim(data.server_name[0]);

    for (i = 0; i < data.error_page.size(); i++)
        _errorPage.insert(getErrPg(trim(data.error_page[i])));

    std::sort(data.locations.begin(), data.locations.end(), comparePrefixLen);
    for (i = 0; i < data.locations.size(); i++)
        _routes.push_back(Route(data.locations[i]));
}

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

void Server::giveListenInfo(Listen &list) const
{
    for (size_t i = 0; i < _port.size(); i++)
        list.insert(std::make_pair(_ip, _port[i]));
}

/////////////////////////////////////////////////////////////////////////

bool Server::matchHost(const Str &host) const
{
    List::const_iterator i;
    for (i = _port.begin(); i != _port.end(); i++)
    {
        if ((_serverName + ":" + *i) == host)
            return true;

        if ((_ip + ":" + *i) == host)
            return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////

bool Server::matchPort(const Str &port) const
{
    List::const_iterator i;
    for (i = _port.begin(); i != _port.end(); i++)
    {
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

void Server::showData()
{
    std::cout << "IP         : [" << _ip                   << "]\n"
              << "ServerName : [" << _serverName           << "]\n" 
              << "Ports      : [" << printmyList(_port)      << "]\n"
              << "Root       : [" << _root                 << "]\n"
              << "ClientBody : [" << _clientBody           << "]\n"
              << "ErrorPage  : \n" << printmyMap(_errorPage)
              << std::endl;
    
    for (size_t i = 0; i < _routes.size(); i++)
    {
        std::cout << "\tRoute[" << i << "]\n\t.............." << std::endl;
        _routes[i].showData();
    }
}
