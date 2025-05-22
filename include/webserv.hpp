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

#include "Client.hpp"
#include "Route.hpp"
#include "Server.hpp"

typedef std::vector<char> Binary;

/////////////////////////////////////////////////////////////
// LOGIC
/////////////////////////////////////////////////////////////

Server serverA();
void processReq(Client &client);

/////////////////////////////////////////////////////////////
// UTILITY FUNCTION
/////////////////////////////////////////////////////////////

int     setNoneBlock(const int &fd);
void    showRawStr(const Str &content);
void    showRawStr(const Binary &content);
bool    isFolderExist(const Str dir);
bool    isValidFile(const Str path);
void    logMsg(const Str &where, Str action, int state);

#endif
