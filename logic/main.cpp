#include "../include/webserv.hpp"

void outgoing(Watchlist_It &i, ClientManager &cManager)
{
	ssize_t byteSent;
	size_t	remainSize = 0;

	Client &client      = cManager.whichClient(i->fd);
	const char *respond = client.getReply(remainSize);

	byteSent = send(i->fd, respond , remainSize, 0);

	if (byteSent > 0)
	{
		if (!client.trackReply(byteSent))  return;
		if (client._keepAlive)
		{ 
			client.reuseFd(); 
			i->events = POLLIN;
			logAction("outgoing | byteSent > 0", "Done sending and resue FD");
			return;
		}
	}
	else
		logError("outgoing | byteSent < 0", "Bad Fd");
	
	logAction("outgoing", "remove Client FD= " + toStr(i->fd));
	cManager.removeClient(i);
}

void incomingRequest(Watchlist_It &i, ClientManager &cManager)
{
	ssize_t byteRead;
	char request[BUFFER_SIZE];
	Client &client = cManager.whichClient(i->fd);
	
	byteRead = recv(i->fd, request, BUFFER_SIZE, 0);

	if (byteRead <= 0)
	{
		if (byteRead < 0)	logError("incomingRequest | byteRead < 0", "Remove bad client FD= " + toStr(i->fd));
		else				logAction("incomingRequest | byteRead = 0", "Client FD= " + toStr(i->fd) + " connection closed");

		cManager.removeClient(i);
		return;
	}

	logAction("incomingRequest | byteRead >", "Read " + toStr(byteRead) + " bytes from Client FD= " + toStr(i->fd));

	if (client.appendReq(request, static_cast<size_t>(byteRead)))
	{
		processReq(client);
		i->events = POLLOUT;
		logAction("incomingRequest", "Request for Client FD= " + toStr(i->fd) + " has been processed");
	}
}

void incomingConnect(Watchlist_It &i, ServerManager &sManager, ClientManager &cManager)
{
	int newFd;
	Server &server = sManager.whichServer(i->fd);

	while (true)
	{
		newFd = -1;

		if ((newFd = accept(i->fd, NULL, NULL)) < 0)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
				logError("accept Status < 0", "Failure");
			break;
		}

		if (setNoneBlock(newFd) < 0)
		{
			close(newFd);
			logError("incomingConnect", "Fail to set FD= " + toStr(newFd) + " to none-blocking"); 
		}

		else
		{
			cManager.addClient(newFd, server);
			logAction("incomingConnect", "Accepted new connection FD= " + toStr(newFd) + " for Server= " + server._serverName);
		}
	}
}

void runServer(Watchlist &watcher, ServerManager &sManager)
{
	Watchlist_It	i;
	int 			pollStatus;
	ClientManager	cManager(watcher);

	while (true)
	{
		pollStatus = poll(&watcher[0], watcher.size(), -1);
		
		if (pollStatus == 0) continue;
		if (pollStatus < 0)
		{
			if (errno == EINTR)
			{ logAction("pollStatus < 0" , "Detect interrupt"); continue; }
			
			logError("pollStatus < 0", "Critical failure");
			exit(EXIT_FAILURE);
		}

		///////////////////////////////////////////////////////////////////////////////////////

		for (i = watcher.begin(); i != watcher.end(); i++)
		{
			if (i->revents & (POLLERR | POLLHUP | POLLNVAL | POLLRDHUP))
			{
				if (sManager.isListenFd(i->fd))
				{
					logError("Revent check", "Critical failure on listen FD= " + i->fd);
					exit(EXIT_FAILURE);
				}
				cManager.removeClient(i);
				logError("Revent check", "Failure on Client FD= " + i->fd);
				continue;
			}

			else if (i->revents & POLLIN)
			{
				if (sManager.isListenFd(i->fd))
					incomingConnect(i, sManager, cManager);
				else
					incomingRequest(i, cManager);
			}

			else if (i->revents & POLLOUT)
				outgoing(i, cManager);
		}
	}
}

int main()
{
	Servers serverList;
	serverList.push_back(server_1());
	serverList.push_back(server_2());
	serverList.push_back(server_3());
	serverList.push_back(server_4());

	Watchlist 		watcher;
	ServerManager	sManager(serverList);
	
	if (!sManager.initListenFd(watcher))
		return EXIT_FAILURE;

	std::cout << GREEN "Server is ready to accept request 👍" RESET << std::endl;
	runServer(watcher, sManager);
}
