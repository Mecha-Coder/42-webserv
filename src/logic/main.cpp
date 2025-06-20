#include "../include/webserv.hpp"

volatile sig_atomic_t _running_ = 1;

void handleSignal(int signum)
{
	std::cout << std::endl;
	(void)signum;
	_running_ = 0;
}

int main(int ac, char **av)
{
	Watchlist 		watcher;
	toml::Table* 	table;
	ServerManager   *sManager =  NULL;

	if (ac > 2)
		return (logError("Input argument", "Expected  : " + Str(av[0]) + " <config.toml>"), 1);
	else if (ac == 1)
		sManager = new ServerManager();
	else
	{
		Str configFile = av[1];
		if ((table = toml::ParseFile(configFile)) == NULL)
			return (logError("Parse config", "Failed to parse the TOML file"), 1);
		
		Config config(*table);
		std::vector<ServerConfig>& servers = config.getServers();
		delete table;

		try { sManager = new ServerManager(servers); }
		catch(const std::exception& e)
		{ return (logError("Parse config", e.what()), 1); }
	}

	logAction("Parse config", "Successful 💪");

	signal(SIGINT, handleSignal);
	if (!sManager->initListenFd(watcher))
		return 1;

	std::cout << CYAN "Server is ready to accept request 👍" RESET
		      << "\n---------------------------------------------"
			<< "--------------------------------------------\n" << std::endl;
	
	runServer(watcher, sManager);
	delete sManager;
	std::cout << RED "Shutting down server 👋" RESET << std::endl;
}
