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
typedef std::set<Str> Ports;

class Server
{
    private:
        Str         _serverName;
        List        _listen;
        ErrorPage   _errorPage;
        Routes      _routes; 

    public:
        const Str       _root;
        const size_t    _clientBody;

        Server(
            const Str       &serverName,
            const List      &listen,
            const ErrorPage &errorPage,
            const Routes    &routes, 
            const Str       &root,
            const size_t    &clientBody
        );

        Route   *findRoute(const Str &uri);
        void    giveListenPort(Ports &portList) const;

        bool    matchHost(const Str &host) const;
        bool    matchPort(const Str &port) const;

        const Str  myErrorPg(const int &code) const;
};

#endif
