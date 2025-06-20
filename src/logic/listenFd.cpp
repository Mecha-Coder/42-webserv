#include "../include/webserv.hpp"

int getReadyList(const IP_Host &pair, struct addrinfo **list)
{
	struct addrinfo hint;
	
	std::memset(&hint, 0, sizeof(hint));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags    = AI_PASSIVE | AI_NUMERICSERV | AI_NUMERICHOST;

	return (getaddrinfo(pair.first.c_str(), pair.second.c_str(), &hint, list) == 0);
}

////////////////////////////////////////////////////////////////////////////////////////

int createSocket(struct addrinfo *addr)
{ return socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol); }

/////////////////////////////////////////////////////////////////////////////////////////

int reuseSocket(int sockfd, int opt = 1)
{ return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); }

/////////////////////////////////////////////////////////////////////////////////////////

int bindToPort(int sockfd, struct addrinfo *addr)
{ return bind(sockfd, addr->ai_addr, addr->ai_addrlen); }

//////////////////////////////////////////////////////////////////////////////////////////

bool checkSockCreation(int sockfd, const IP_Host &pair)
{
	struct sockaddr_in check;
	socklen_t addrlen = sizeof(check);

	if (getsockname(sockfd, (struct sockaddr *)&check, &addrlen) == -1)
		return false;
		
	Str _port = toStr(ntohs(check.sin_port));
	uint32_t _ip = ntohl(check.sin_addr.s_addr);

	return ((_port == pair.second) && (_ip != 0xFFFFFFFFUL) && (_ip != 0x00000000UL));
}

//////////////////////////////////////////////////////////////////////////////////////////

int create_listenFd(const IP_Host &pair)
{
	int sockfd = -1;
    struct addrinfo *list = NULL, *addr = NULL;

	Str where =  pair.first + ":" + pair.second;

	if (getReadyList(pair, &list)) 
	{
		for (addr = list; addr != NULL; addr = addr->ai_next)
    	{
			if (addr->ai_family != AF_INET || addr->ai_socktype != SOCK_STREAM)
                continue;

        	sockfd = createSocket(addr);
        	if (sockfd < 0 ) continue;
        
			if (reuseSocket(sockfd) < 0 || setNoneBlock(sockfd) <  0 
				|| bindToPort(sockfd, addr) < 0 || listen(sockfd, SOMAXCONN) < 0)
        	{
				close(sockfd);
				continue; 
			}
       		break;
    	}
		freeaddrinfo(list);
		
		if (addr)
		{
			if (checkSockCreation(sockfd, pair)) 
				return (logAction(where, "Created listen FD=" + toStr(sockfd)), sockfd);
			close(sockfd);
		}
	}
    return (logError(where, "Failed to create listen FD "), -1);
}

//////////////////////////////////////////////////////////////////////////////////////////

/*
int main()
{
	std::cout << YELLOW "Invalid host:port\n==========\n" RESET << std::endl;
	
	create_listenFd(Pair(std::make_pair("127.0.0.1", "abc")));
	create_listenFd(Pair(std::make_pair("nonexistent.domain", "80")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "99999")));
	create_listenFd(Pair(std::make_pair("localhost", "")));
	create_listenFd(Pair(std::make_pair("256.256.256.256", "8080")));
	create_listenFd(Pair(std::make_pair("::gggg", "80")));
	create_listenFd(Pair(std::make_pair("[::1]", "8080")));
	create_listenFd(Pair(std::make_pair("   ", "8080")));
	create_listenFd(Pair(std::make_pair("example.invalid", "9000")));
	create_listenFd(Pair(std::make_pair("1234", "8000")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "-1")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "  ")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "00080")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "65536")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "70000")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "abc123")));
	create_listenFd(Pair(std::make_pair("", "70000")));
	create_listenFd(Pair(std::make_pair("", "")));
	create_listenFd(Pair(std::make_pair("", "8080")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "22"))); 
	create_listenFd(Pair(std::make_pair("127.0.0.1", "0")));
	create_listenFd(Pair(std::make_pair("::", "8000")));
	create_listenFd(Pair(std::make_pair("::1", "8080")));
	create_listenFd(Pair(std::make_pair("localhost", "00080")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "1")));
	create_listenFd(Pair(std::make_pair("255.255.255.255", "8080")));
	create_listenFd(Pair(std::make_pair("0.0.0.0", "8000")));


	std::cout << YELLOW "\n\nValid host:port\n==========\n" RESET << std::endl;
	
	create_listenFd(Pair(std::make_pair("168.58.90.5", "50000")));
	create_listenFd(Pair(std::make_pair("254.10.20.30", "9100")));
	create_listenFd(Pair(std::make_pair("168.58.90.5", "8090")));
	create_listenFd(Pair(std::make_pair("180.5.2.11", "8000")));
	create_listenFd(Pair(std::make_pair("localhost", "8080")));
	create_listenFd(Pair(std::make_pair("173.5.5.5", "10000")));
	create_listenFd(Pair(std::make_pair("172.5.5.5", "4000")));
	create_listenFd(Pair(std::make_pair("172.80.245.90", "5000")));
	create_listenFd(Pair(std::make_pair("127.0.0.1", "10000")));
	create_listenFd(Pair(std::make_pair("127.2.2.1", "9000")));
	create_listenFd(Pair(std::make_pair("127.66.62.61", "23000")));
}
*/

//////////////////////////////////////////////////////////////////////////////////////////

/*
	struct addrinfo hint, *list;

	std::memset(&hint, 0, sizeof(hint));
	hint.ai_family   = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags    = AI_PASSIVE;

	if (getaddrinfo("127.0.0.1", "65536", &hint, &list) == 0)
		std::cout << "Passed" << std::endl;
	else
		std::cout << "Failed" << std::endl;
*/
