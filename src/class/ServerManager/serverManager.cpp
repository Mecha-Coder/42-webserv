#include "../../include/ServerManager.hpp"

int create_listenFd(const IP_Host &listen);

////////////////////////////////////////////////////////////////////////////////////////////////////

ServerManager::ServerManager()
{
	LocationConfig route;
	route.index.push_back("index.html");
	route.prefix = "/";

	ServerConfig server;
	server.host = "127.0.0.1";
	server.port.push_back("8080");
	server.server_name.push_back("webserv.com");
	server.root = "./src/default";
	server.locations.push_back(route);

	_serverList.push_back(Server(server));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ServerManager::ServerManager(std::vector<ServerConfig>& data)
{
	size_t i;
	for (i = 0; i < data.size(); i++)
		_serverList.push_back(Server(data[i]));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool ServerManager::initListenFd(Watchlist &watcher)
{
	int listenFd;
	Listen list;
	Listen::iterator j;

	for (size_t i = 0; i < _serverList.size(); i++)
		_serverList[i].giveListenInfo(list);

	for (j = list.begin(); j != list.end(); j++)
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
