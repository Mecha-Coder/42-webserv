#include "../include/webserv.hpp"

void outgoing(Watchlist_It &i, ClientManager &cManager)
{
	Str		where;
	ssize_t byteSent;
	size_t	remainSize;

	Client &client      = cManager.whichClient(i->fd);
	const char *respond = client.getReply(remainSize);

	byteSent = send(i->fd, respond , remainSize, 0);
	where	 = "Outgoing | byteSend = " + toStr(byteSent);

	if (byteSent <= 0)
		logError(where, "Remove bad clientFD= " + toStr(i->fd));
	else
	{
		logNote(where, "Write response to " + toStr(i->fd));

		if (!client.trackReply(byteSent))
			return;

		logNote(where, "Done sending response to  " + toStr(i->fd));

		if (client._keepAlive)
		{
			client.reuseFd(); 
			i->events = POLLIN;
			logAction("where", "Reuse clientFd= " + toStr(i->fd));
			return;
		}
		
		logNote(where, "No reuse. Proceed to close clientFd= " + toStr(i->fd));
	}	
	cManager.removeClient(i);
}

void incomingRequest(Watchlist_It &i, ClientManager &cManager)
{
	Client &client = cManager.whichClient(i->fd);

	Str		where;
	ssize_t	byteRead;
	char 	request[BUFFER_SIZE];
	
	byteRead = recv(i->fd, request, BUFFER_SIZE, 0);
	where    = "Incoming Request | byteRead = " + toStr(byteRead);

	if (byteRead > 0)
	{
		logNote(where, "Read request for " + toStr(i->fd));

		if (client.appendReq(request, static_cast<size_t>(byteRead)))
		{
			processReq(client);
			logAction(where, "Request for clientFd= " + toStr(i->fd) + " has been processed");
			i->events = POLLOUT;
		}
	}
	else
	{
		if (byteRead < 0)
			logError(where, "Remove bad clientFD= " + toStr(i->fd));
		else
			logNote(where, "Connection closed for clientFD= " + toStr(i->fd));
		cManager.removeClient(i);
	}
}

void incomingConnect(Watchlist_It &i, Server &server, ClientManager &cManager)
{
	int newFd;
	Str where;

	while (true)
	{
		newFd = accept(i->fd, NULL, NULL);
		where = "Incoming Connection | Accept Status = " + toStr(newFd); 

		if (newFd > 0)
		{
			if (setNoneBlock(newFd) < 0)
			{
				logError(where, "Fail to set FD= " + toStr(newFd) + " to none-blocking");
				close(newFd);
				continue;
			}
			cManager.addClient(newFd, server);
		}
		else
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				logNote(where, "No pending connection");
			else
				logError(where, "Accept failed: " + Str(strerror(errno)));
			break;
		}
	}
}

void checkEvents(Watchlist &watcher, ServerManager &sManager)
{
	Str				where;
	Watchlist_It	i;
	ClientManager	cManager(watcher);

	for (i = watcher.begin(); i != watcher.end(); i++)
	{
		if (i->revents & (POLLERR | POLLHUP | POLLNVAL | POLLRDHUP))
		{
			where = "Check events | Error";

			std::cout << "Error: fd= " << i->fd << " event= " << i->events << " revents= " << i->revents << std::endl;

			if (sManager.isListenFd(i->fd))
			{
				logError(where, "Critical: Event error on listenFd= " + toStr(i->fd));
				exit(EXIT_FAILURE);
			}

			logError(where, "Event error on clientFd= " + toStr(i->fd));
			cManager.removeClient(i);
		}

		else if (i->revents & POLLIN)
		{
			std::cout << "POLLIN: fd= " << i->fd << " event= " << i->events << " revents= " << i->revents << std::endl;

			if (sManager.isListenFd(i->fd))
				incomingConnect(i, sManager.whichServer(i->fd), cManager);
			else
				incomingRequest(i, cManager);
		}

		else if (i->revents & POLLOUT)
		{
			std::cout << "POLLOUT: fd= " << i->fd << " event= " << i->events << " revents= " << i->revents << std::endl;
			outgoing(i, cManager);
		}
		
		else if (i->revents == 0)
		{
			std::cout << "No event: fd= " << i->fd << " event= " << i->events << " revents= " << i->revents << std::endl;
		}

		else
		{
			std::cout << "Something else: fd= " << i->fd << " event= " << i->events << " revents= " << i->revents << std::endl; 
			logNote("Check events", "Detected unknown events"); 
		}
	}
}

void runServer(Watchlist &watcher, ServerManager &sManager)
{	
	Str		where;
	int 	pollRet;
	
	while (true)
	{
		pollRet = poll(&watcher[0], watcher.size(), -1);
		where   = "Polling status= " + toStr(pollRet);	
		
		if (pollRet > 0)
			checkEvents(watcher, sManager);

		else if (pollRet == 0)
			logNote(where, "Timeout, no events");
		
		else if (pollRet < 0)
		{
			if (errno != EINTR)
			{
				logError(where, "Critical: Poll failed: " + Str(strerror(errno)));
				exit(EXIT_FAILURE);
			}
			logAction(where, strerror(errno));
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

	std::cout << CYAN "Server is ready to accept request 👍" RESET << std::endl;
	runServer(watcher, sManager);
}
