#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <fstream>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <cctype>
#include <algorithm>
#include <csignal>
#include <stdexcept>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>

#include "constant.hpp"
#include "Client.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include "ServerManager.hpp"
#include "ClientManager.hpp"
#include "../toml/Toml.hpp"
#include "Config.hpp"

template <typename T> Str toStr(T value);
extern volatile sig_atomic_t _running_;

/************************************************************/
// RUNING
/************************************************************/

void	processReq(Client &client);
void 	runServer(Watchlist &watcher, ServerManager *sManager);

/************************************************************/
// UTILS
/************************************************************/

int		setNoneBlock(const int &fd);
void	showHttp(const Str &content);
bool	readFile(const Str& filename, Str &content);
void	logError(const Str &where, Str error);
void	logAction(const Str &where, Str action);
void    logNote(const Str &where, Str note);

#endif
