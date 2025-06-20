#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include "Route.hpp"

typedef std::string Str;
typedef std::vector<Str> List;
typedef std::vector<Route> Routes;
typedef std::map<int, Str> ErrorPage;
typedef std::pair<Str, Str> IP_Host;
typedef std::set<IP_Host> Listen;

class Server
{
    private:
        Str         _ip;
        Str         _serverName;
        List        _port;
        ErrorPage   _errorPage;
        Routes      _routes;

    public:
        Str       _root;
        size_t    _clientBody;

        Server(ServerConfig &data);

        Route   *findRoute(const Str &uri);
        void    giveListenInfo(Listen &list) const;

        bool    matchHost(const Str &host) const;
        bool    matchPort(const Str &port) const;

        const Str  myErrorPg(const int &code) const;
};

#endif
