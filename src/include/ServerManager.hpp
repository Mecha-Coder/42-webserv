#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <unistd.h>
#include <poll.h>
#include "Server.hpp"

typedef std::vector<int> Fd;
typedef std::vector<Server> Servers;
typedef std::vector<struct pollfd> Watchlist;

class ServerManager
{
    private:
        Servers _serverList;
        Fd      _listenFds;
         
    public:
        ServerManager();
        ServerManager(std::vector<ServerConfig>& data);

        bool initListenFd(Watchlist &watcher);
        bool isListenFd(int fd);

        Server *whichServer(const Str &host);
        void showData();
};

#endif