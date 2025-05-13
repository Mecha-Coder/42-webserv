#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "Route.hpp"

typedef std::string Str;
typedef std::vector<Str> Address;
typedef std::map<int, Str> ErrorPage;
typedef std::vector<Route> Routes;
typedef std::vector<std::pair<Str, Str> > IP_Host;

class Server
{
    private:
        Address     _listen;
        ErrorPage   _errorPage;
        Routes      _routes; 

    public:
        const Str     _serverName;
        const Str     _mainRoot;
        const int     _clientBodySize;

        Server(
            const Str &serverName,
            const Str &mainRoot,
            const int &clientBodySize,
            const Address &listen,
            const ErrorPage &errorPage,
            const Routes &routes 
        );

        const Route     *findRoute(const Str uri) const;
        const Str       myErrorPg(const int &code) const;
        bool            isMyAddr(const Str &host) const;
        void            giveMyAddr(IP_Host &addrList) const;
        void            showData() const;

};

#endif