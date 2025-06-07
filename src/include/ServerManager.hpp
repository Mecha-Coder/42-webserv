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

typedef std::vector<Server> Servers;
typedef std::map<int , Server*> Fd_Server;
typedef std::vector<struct pollfd> Watchlist;

class ServerManager
{
    private:
        Servers     _serverList;
        Fd_Server   _listenMap;
         
    public:
        ServerManager();
        ServerManager(Servers &serverList);

        void showData() const;
        bool initListenFd(Watchlist &watcher);
        bool isListenFd(int fd);

        Server &whichServer(int fd);
};

#endif