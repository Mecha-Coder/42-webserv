#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#define YELLOW "\033[33m"
#define RESET  "\033[0m"

#include "Client.hpp"
#include "Route.hpp"
#include "Server.hpp"

void processReq(Client &client);
Server serverA();

#endif