#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#define YELLOW  "\033[33m"
#define CYAN    "\033[0;36m"
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define RESET   "\033[0m"

#include <fcntl.h>
#include <sys/stat.h> 
#include <fstream>
#include <cstdio>
#include <sstream>

#include "Client.hpp"
#include "Route.hpp"
#include "Server.hpp"

template <typename T> Str toStr(T value);

/************************************************************/
// TESTING
/************************************************************/

Server serverA();
void processReq(Client &client);

/************************************************************/
// UTILS
/************************************************************/

int setNoBlock(const int &fd);
void showHttp(const Str &content);
bool readFile(const Str& filename, Str &content);

#endif
