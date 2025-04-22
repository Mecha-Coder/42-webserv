#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <string>
#include <vector>

#define PORT 8080
#define MAX_CLIENTS 1024
#define BUFFER_SIZE 4096
#define MAX_REQUEST_SIZE 16384

extern volatile sig_atomic_t server_run;
typedef std::string str;
typedef struct pollfd t_pollfd; 

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[33m"
#define RESET  "\033[0m"


typedef struct Client 
{
    int fd;
    str buffer;
} t_client;

bool runServer(t_pollfd *FD, int pollSize);
bool initialize(t_pollfd *FD);

bool err_msg(str const &msg, int code);
int set_nonblocking(int const &fd);

#endif
