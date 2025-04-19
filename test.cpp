#include <cerrno> 
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>    // for close()
#include <sys/epoll.h> // for epoll_create1()
#include <cstdlib>

bool err_msg(std::string const &where, int code)
{
    std::cout << "epoll_create:" << strerror(code);
    return 1; 
}

int main() 
{
    int epollFD, monitorFD;
    struct epoll_event event;
    
    monitorFD = STDIN_FILENO;

    if (epollFD = epoll_create(1), epollFD == -1) 
        return err_msg("epoll_create: ", errno);

    // Set event
    event.events = EPOLLIN;
    event.data.fd = monitorFD;

    if (epoll_ctl(epollFD, EPOLL_CTL_ADD, monitorFD, &event) == -1)
        return (close(epollFD), err_msg("epoll_create: ", errno));
    
    std::cout << "Successfully added fd for monitoring\n";
    close(epollFD);
}