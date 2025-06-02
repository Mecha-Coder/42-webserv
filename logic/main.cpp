#include "../include/webserv.hpp"

void outgoing(Watchlist_It &i, ClientManager &cManager)
{

}

void incomingRequest(Watchlist_It &i, ClientManager &cManager)
{
	int acceptStatus;
	
}

void incomingConnect(Watchlist_It &i, ServerManager &sManager, ClientManager &cManager)
{
	int newfd;
	Server &server = sManager.whichServer(i->fd);

	while (true)
	{
		newfd = -1;

		if ((newfd = accept(i->fd, NULL, NULL)) < 0)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
				logError("accept Status < 0", "Failure");
			break;
		}


		cManager.addClient(newfd, server);
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
