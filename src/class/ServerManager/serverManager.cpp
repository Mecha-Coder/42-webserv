#include "../../include/ServerManager.hpp"

int create_listenFd(const Pair &pair, const Str &website);

////////////////////////////////////////////////////////////////////////////////////////////////////

ServerManager::ServerManager()
{
	Address listen;
    listen.push_back("127.0.0.1:8080");

    ErrorPage errorPg;

    Str mainRoot = "./src/default";
    CGI noCgi;
    
    CGI allCgi;

    Method onlyGET; 
        onlyGET.push_back("GET");

    //------------------------------------------------------------

    Route route1 (
        "/",
        mainRoot,
        "",
        "index.html",
        "",
        false,
        onlyGET,
        noCgi
    );

    Routes routes;
    routes.push_back(route1);

	_serverList.push_back(  Server (
        "www.webservDefault.my",
        mainRoot,
        100000,
        listen,
        errorPg,
        routes
    ));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ServerManager::ServerManager(Servers &serverList)
: _serverList(serverList) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ServerManager::showData() const
{
	for (size_t i = 0; i < _serverList.size(); i++)
	{
		std::cout << "Server [" << i << "] ******************************************************\n" 
				  << std::endl;
		_serverList[i].showData();		
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool ServerManager::initListenFd(Watchlist &watcher)
{
	int listenFd;
	IP_Host addr;
	IP_Host::iterator j;

	for (size_t i = 0; i < _serverList.size(); i++)
	{
		listenFd = -1;
		_serverList[i].giveMyAddr(addr);

		for (j = addr.begin(); j != addr.end(); j++)
		{ 
			listenFd = create_listenFd(*j, _serverList[i]._serverName);
			if (listenFd == -1)
				return  false;
			
			_listenMap.insert(std::make_pair(listenFd, &_serverList[i]));

			struct pollfd pfd;
			pfd.fd = listenFd;
			pfd.events = POLLIN;
			pfd.revents = 0;
			watcher.push_back(pfd);
		}
		addr.clear();		
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool ServerManager::isListenFd(int fd)
{
	Fd_Server::const_iterator i;
	for (i = _listenMap.begin() ; i != _listenMap.end(); i++)
	{
		if (i->first == fd)
			return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Server &ServerManager::whichServer(int fd)
{
	Fd_Server::iterator i;
	for (i = _listenMap.begin() ; i != _listenMap.end(); i++)
	{
		if (i->first == fd)
			break;
	}
	if (i == _listenMap.end())
		throw std::out_of_range("ServerManager::whichServer::Fd not found");
	return *(i->second);
}
