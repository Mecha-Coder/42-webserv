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
typedef std::pair<Str, Str> Pair;
typedef std::vector<Pair> IP_Host;

class Server
{
    private:
        Address     _listen;
        ErrorPage   _errorPage;
        Routes      _routes; 

    public:
        const Str       _serverName;
        const Str       _mainRoot;
        const size_t    _clientBodySize;

        Server(
            const Str &serverName,
            const Str &mainRoot,
            const size_t &clientBodySize,
            const Address &listen,
            const ErrorPage &errorPage,
            const Routes &routes 
        );

        Route     *findRoute(const Str uri);
        const Str  myErrorPg(const int &code) const;
        void       giveMyAddr(IP_Host &addrList) const;
        void       showData() const;

        // bool       isMyAddr(const Str &host) const;
};

#endif