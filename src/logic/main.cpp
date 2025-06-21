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
	Str				where = "Parse config";

	if (ac > 2)
		return (logError("Invalid argument", "Expected: " + Str(av[0]) + " <config.toml>"), 1);
	else if (ac == 1)
	{
		sManager = new ServerManager();
		logAction("", "Use default config 😎");
	}
	else
	{
		Str configFile = av[1];

		if (configFile.find(".toml") == configFile.npos)
			return (logError(where, "Invalid TOML file"), 1);
		if ((table = toml::ParseFile(configFile)) == NULL)
			return (logError(where, "Failed"), 1);
		

		Config config(*table);
		std::vector<ServerConfig>& servers = config.getServers();
		delete table;
		if (servers.empty())
		{
			return (logError(where, "Server data is empty"), 1);
			return 1;
		}
			

		try { sManager = new ServerManager(servers); }
		catch(const std::exception& e)
		{ return (logError(where, e.what()), 1); }
		logAction("Parse config", "Successful 💪");
	}

	//sManager->showData(); // Remove after troubleshooting

	signal(SIGINT, handleSignal);
	signal(SIGTSTP, handleSignal);
	signal(SIGQUIT, handleSignal);

	if (!sManager->initListenFd(watcher))
		return (delete sManager, 1);

	std::cout << CYAN "Server is ready to accept request 👍" RESET
		      << "\n---------------------------------------------"
			<< "--------------------------------------------\n" << std::endl;
	
	runServer(watcher, sManager);
	delete sManager;
	std::cout << RED "Shutting down server 👋" RESET << std::endl;
}
