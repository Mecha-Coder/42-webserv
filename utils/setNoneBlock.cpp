#include "../include/webserv.hpp"

/* 
Purpose:
- Set FD to none-blocking mode
- Function like accept(), recv() & send() will not freeze

_______________________________________________________________________________

Return:
     0 ➡️ Successful in setting to non blocking
    -1 ➡️ Fail
*/
int setNoneBlock(const int &fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) 
        return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
