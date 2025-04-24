#ifndef HEADER_HPP
#define HEADER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdio>

int set_nonblocking(int fd);

#endif
