#include "../include/webserv.hpp"

void runServer()
{
	ClientManager	cManager;

	while (true)
	{
		pollStatus = poll(&watcher[0], watcher.size(), -1);
		
		if (pollStatus == 0) 
		{ logAction("pollStatus == 0" , "Retry"); continue; }
		
		if (pollStatus < 0)
		{
			if (errno == EINTR)
			{ 
				logAction("pollStatus < 0" , "Detect interrupt"); continue; 
			}
			else
			{
				logError("pollStatus < 0", "Critical failure");
				exit(EXIT_FAILURE);
			}
		}

		Watchlist::iterator i;
		for (i = watcher.begin(); i != watcher.end(); i++)
		{
			if (i->revents == 0)
			{
				logAction("revents == 0" , "No event for FD= " + toStr(watcher[i].fd)); 
				continue; 
			}

			if (i->revents & (POLLERR | POLLHUP | POLLNVAL | POLLRDHUP))
			{
				if (sManager.isListenFd(i->fd))
				{
					logError("Revent check", "Critical failure on listen FD");
					exit(EXIT_FAILURE);
				}

				i = --(watcher.erase(i));
				logError("Revent check", "Failure on listen FD");
				continue;
			}

			if (i->revents & POLLOUT)
			{
				if (sManager.isListenFd(i->fd))
				{

				}
				else
				{

				}
				continue;
			}

			if (i->revents & POLLIN)
			{

			}
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
