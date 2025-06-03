#include "../../include/ClientManager.hpp"

ClientManager::ClientManager(Watchlist &watcher)
: _watcher(watcher){}

//////////////////////////////////////////////////////////////////////////////////////

void ClientManager::removeClient(int clientFd, size_t &index)
{
    _clientList.erase(clientFd);
    _watcher.erase(_watcher.begin() + index);
    index--;

    logAction("Client Manager", "Remove clientFd= " + toStr(clientFd));
}

//////////////////////////////////////////////////////////////////////////////////////

void ClientManager::addClient(int clientFd, Server &server)
{
    std::cout << "Before ClientList size= " << _clientList.size() << std::endl;
    _clientList.insert(std::make_pair(clientFd, Client(server)));
    std::cout << "After ClientList size= " << _clientList.size() << " FD= " << _clientList.begin()->first << std::endl;


    struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_watcher.push_back(pfd);

    logAction("Client Manager", "Added new clientFd= " + toStr(clientFd) + ", link Server= " + server._serverName);
}

//////////////////////////////////////////////////////////////////////////////////////

Client &ClientManager::whichClient(int clientFd)
{

    std::cout << "whichClient: ClientList size= " << _clientList.size() << " FD= " << _clientList.begin()->first << std::endl;

    Clients::iterator i;
    for (i = _clientList.begin(); i != _clientList.end(); i++)
    {
        if (i->first == clientFd)
        {
            std::cout << GREEN "Found " + toStr(i->first) << std::endl; 
            break;
        }   
    }
    if (i == _clientList.end())
		throw std::out_of_range("ClientManager: whichClient: Fd= " + toStr(clientFd) + " not found");
    return i->second;
}
