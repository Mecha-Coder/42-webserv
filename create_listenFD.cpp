#include "../_include/webserv.hpp"

int create_socket(struct addrinfo *addr)
{return socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);}

int reuse_socket(int sockfd, int reuse = 1)
{return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));}

int bind_to_port(int sockfd, struct addrinfo *addr)
{return bind(sockfd, addr->ai_addr, addr->ai_addrlen);}

struct addrinfo *get_addr_list(const char *IP, const char *port)
{
    int status;
    struct addrinfo hint, *result;

    std::memset(&hint, 0, sizeof(hint));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags    = AI_PASSIVE;

    status = getaddrinfo(IP, port, &hint, &result);
    if (status)
        return NULL;
    return result;
}

int socketCreation(const char *IP, const char *port)
{
    int sockfd;
    struct addrinfo *addr, *result;

    result = get_addr_list(IP, port);
    for (addr = result; addr != NULL; addr = addr->ai_next)
    {
        sockfd = create_socket(addr);
        if (sockfd == -1)
            continue;
        if (reuse_socket(sockfd) == -1 || bind_to_port(sockfd, addr) == -1 || listen(sockfd, SOMAXCONN) == -1)
        {
            close(sockfd);
            continue;
        }
        break;
    }
    if (addr == NULL) 
    {
        std::cerr << "Failed to create listening FD for " << IP << ":" << port << "\n";
        return -1;
    }
    freeaddrinfo(result);
    std::cout << "Socket " << sockfd << " listening FD on " << IP << ":" << port << "\n";
    return sockfd;
}

bool createListenFD(ServerManager &servers)
{
    int sockfd;
    std::vector<std::pair<str, str> > pair;
    servers.getListenPair(pair);

    std::vector<std::pair<str, str> >::iterator i = pair.begin();
    for (; i != pair.end(); i++)
    {
        sockfd = socketCreation(i->first.c_str(), i->second.c_str());
        if (sockfd == -1) 
        {
            servers.closeAllFD();
            return false;
        }
        servers.addListenFD(sockfd);
    }
    return true;    
}
