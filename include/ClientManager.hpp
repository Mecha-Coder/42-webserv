#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "Client.hpp"

typedef std::map<int, Client> Clients;

class ClientManager
{
    private:
        Servers     _serverList;
        Fd_Server   _listenMap;
         
    public:
};

#endif