#include "../include/webserv.hpp"

void outgoing(struct pollfd &watch, ClientManager &cManager, size_t &index)
{
	Str		where;
	ssize_t byteSent;
	size_t	remainSize;

	Client &client      = cManager.whichClient(watch.fd);
	const char *respond = client.getReply(remainSize);

	byteSent = send(watch.fd, respond , remainSize, 0);
	where	 = "Outgoing | byteSend = " + toStr(byteSent);

	if (byteSent <= 0)
		logError(where, "Remove bad clientFD= " + toStr(watch.fd));
	else
	{
		logNote(where, "Write response to " + toStr(watch.fd));

		if (!client.trackReply(byteSent))
			return;

		logNote("Outgoing", "Done sending response to  " + toStr(watch.fd));

		if (client._keepAlive)
		{
			client.reuseFd(); 
			watch.events = POLLIN;
			logAction("Outgoing", "Reuse clientFd= " + toStr(watch.fd));
			return;
		}
		
		logNote(where, "No reuse. Proceed to close clientFd= " + toStr(watch.fd));
	}	
	cManager.removeClient(watch.fd, index);
}

void incomingRequest(struct pollfd &watch, ClientManager &cManager, size_t &index)
{
	Client &client = cManager.whichClient(watch.fd);

	Str		where;
	ssize_t	byteRead;
	char 	request[BUFFER_SIZE];
	
	byteRead = recv(watch.fd , request, BUFFER_SIZE, 0);
	where    = "Incoming Request | byteRead = " + toStr(byteRead);

	if (byteRead > 0)
	{
		logNote(where, "Read request on FD= " + toStr(watch.fd));

		if (client.appendReq(request, static_cast<size_t>(byteRead)))
		{
			processReq(client);
			logAction("Incoming Request", "Request for clientFd= " + toStr(watch.fd) + " has been processed");
			watch.events = POLLOUT;
		}
	}
	else
	{
		if (byteRead < 0)
			logError(where, "Remove bad clientFD= " + toStr(watch.fd));
		else
			logNote(where, "Connection closed for clientFD= " + toStr(watch.fd));
		cManager.removeClient(watch.fd, index);
	}
}

void incomingConnect(int listenFd, Server &server, ClientManager &cManager)
{
	int newFd;
	Str where;

	while (true)
	{
		newFd = accept(listenFd, NULL, NULL);
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

void checkEvents(Watchlist &watcher, ServerManager &sManager, ClientManager &cManager)
{
	Str				where;
	
	for (size_t i = 0; i < watcher.size(); i++)
	{
		if (watcher[i].revents & (POLLERR | POLLHUP | POLLNVAL | POLLRDHUP))
		{
			where = "Check events | Error";
			if (sManager.isListenFd(watcher[i].fd))
			{
				logError(where, "Critical: Event error on listenFd= " + toStr(watcher[i].fd));
				exit(EXIT_FAILURE);
			}
			logError(where, "Event error on clientFd= " + toStr(watcher[i].fd));
			cManager.removeClient(watcher[i].fd, i);
		}

		else if (watcher[i].revents & POLLIN)
		{
			where = "Check events | POLLIN";
			logNote(where, "FD= " + toStr(watcher[i].fd));
			
			if (sManager.isListenFd(watcher[i].fd))
				incomingConnect(watcher[i].fd, sManager.whichServer(watcher[i].fd), cManager);
			else
				incomingRequest(watcher[i], cManager, i);
		}

		else if (watcher[i].revents & POLLOUT)
		{
			where = "Check events | POLLOUT";
			logNote(where, "FD= " + toStr(watcher[i].fd));
			outgoing(watcher[i], cManager, i);
			
		}
		
		else if (watcher[i].revents == 0)
			continue;
		
		else
			logNote("Check events", "Detected unknown events Fd= " + toStr(watcher[i].fd));
		
		std::cout << "---------------------------------------------"
				   << "--------------------------------------------\n" << std::endl;
	}
}

void runServer(Watchlist &watcher, ServerManager &sManager)
{	
	Str				where;
	int 			pollRet;
	ClientManager	cManager(watcher);

	while (true)
	{
		pollRet = poll(&watcher[0], watcher.size(), -1);
		where   = "Polling status= " + toStr(pollRet);	
		
		if (pollRet > 0)
			checkEvents(watcher, sManager, cManager);

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

	Watchlist 		watcher;
	ServerManager	sManager(serverList);
	
	if (!sManager.initListenFd(watcher))
		return EXIT_FAILURE;

	std::cout << CYAN "Server is ready to accept request 👍" RESET
		      << "\n---------------------------------------------"
			<< "--------------------------------------------\n" << std::endl;
	runServer(watcher, sManager);
}
