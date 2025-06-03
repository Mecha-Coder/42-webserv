#include "../../include/ClientManager.hpp"

ClientManager::ClientManager(Watchlist &watcher)
: _watcher(watcher){}

//////////////////////////////////////////////////////////////////////////////////////

void ClientManager::removeClient(Watchlist_It &i)
{
    _clientList.erase(i->fd);
    i = --(_watcher.erase(i));
}

//////////////////////////////////////////////////////////////////////////////////////

void ClientManager::addClient(int clientFd, Server &server)
{
    _clientList.insert(std::make_pair(clientFd, Client(server)));
    
    struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_watcher.push_back(pfd);
}

//////////////////////////////////////////////////////////////////////////////////////

Client &ClientManager::whichClient(int clientFd)
{
    Clients::iterator i;
    for (i = _clientList.begin(); i != _clientList.end(); i++)
    {
        if (i->first == clientFd)
            break;
    }
    return i->second;
}
