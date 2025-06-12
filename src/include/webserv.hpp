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

template <typename T> Str toStr(T value);

/************************************************************/
// TESTING
/************************************************************/

Server	server_1();
Server	server_2();
Server	server_3();
Server	server_4();
Server	server_5();
void	processReq(Client &client);

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
