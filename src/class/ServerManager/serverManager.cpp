#include "../../include/ServerManager.hpp"

int create_listenFd(const Str &port);

////////////////////////////////////////////////////////////////////////////////////////////////////

ServerManager::ServerManager()
{
	Routes routes;
	ErrorPage errorPg;
	List listen, noCgi, onlyGet;
	
    listen.push_back("8080");
    onlyGet.push_back("GET");

    Route route1 (
		onlyGet,
		noCgi,
        "/",
        "",
        "index.html",
        false,
        false
    );
    routes.push_back(route1);

	_serverList.push_back(
		Server(
			"webservDefault.com",
			listen,
			errorPg,
			routes,
			"./src/default",
			100000
    ));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ServerManager::ServerManager(Servers &serverList)
: _serverList(serverList) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool ServerManager::initListenFd(Watchlist &watcher)
{
	int listenFd;
	Ports portList;
	Ports::iterator j;

	for (size_t i = 0; i < _serverList.size(); i++)
		_serverList[i].giveListenPort(portList);	

	for (j = portList.begin(); j != portList.end(); j++)
	{
		listenFd = -1;
		listenFd = create_listenFd(*j);
		if (listenFd == -1)
			return  false;
		
		_listenFds.push_back(listenFd);

		struct pollfd pfd;
		pfd.fd = listenFd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		watcher.push_back(pfd);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool ServerManager::isListenFd(int fd)
{
	for (size_t i = 0; i < _listenFds.size(); i++)
	{
		if (_listenFds[i] == fd)
			return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Server *ServerManager::whichServer(const Str &host)
{
	Servers::iterator i;
	Str port; size_t pos;

	if ((pos = host.find(":")) != host.npos)
		port = host.substr(pos + 1);

	for (i = _serverList.begin(); i != _serverList.end(); i++)
	{
		if (i->matchHost(host))
			return &(*i);
	}

	for (i = _serverList.begin(); i != _serverList.end(); i++)
	{
		if (i->matchPort(port))
			return &(*i);
	}

	return NULL;
}
