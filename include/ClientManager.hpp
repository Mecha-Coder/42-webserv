#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "Client.hpp"

typedef std::map<int, Client> Clients;
typedef std::vector<struct pollfd> Watchlist;

class ClientManager
{
    private:
        Clients     _clientList;
        Watchlist   &_watcher;
         
    public:
        ClientManager(Watchlist &watcher);

        void addClient(int clientFd, Server &server);
        Client &whichClient(int clientFd);
        void removeClient(int clientFd, size_t &index);
};

#endif