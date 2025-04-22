#include "../include/header.hpp"

int create_listen_sock()
{return socket(AF_INET, SOCK_STREAM, 0);}

int make_sock_reusable(int &listenFD, int reuse = 1)
{return setsockopt(listenFD, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));}

int bind_sock(int &listenFD)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    return bind(listenFD, (sockaddr*)&addr, sizeof(addr));
}

void add_listenFD_2_poll(int &listenFD, t_pollfd *FD)
{
    FD[0].fd = listenFD;
    FD[0].events = POLLIN;
}

bool initialize(t_pollfd *FD)
{
    int listenFD;
    
    if (listenFD = create_listen_sock(), listenFD)
        return err_msg("", errno);
    if (make_sock_reusable(listenFD) || bind_sock(listenFD) || listen(listenFD, SOMAXCONN) || set_nonblocking(listenFD))
    {
        close(listenFD);
        return err_msg("", errno);
    }
    add_listenFD_2_poll(listenFD, FD);
    return true;
}
