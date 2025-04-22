#include "../include/header.hpp"

bool runServer(t_pollfd *FD, int pollSize)
{
    int status;

    status = poll(FD, pollSize, 200);
    if ((status == -1 && errno == EINTR) || status == 0)
        return true;

    if (status == -1 && errno != EINTR)
        return err_msg("", errno);
    
    for (int i = 0; i < pollSize ; i++)
    {
        if (FD[i].revents == 0) continue;

        if (FD[i].revents & (POLLERR | POLLHUP | POLLNVAL))
        {
            std::cout << "Closing bad fd " << FD[i].fd << "\n";
            close(FD[i].fd);
            FD[i] = FD[--pollSize];
            i--;
            continue;
        }

        if (FD[i].revents & POLLIN)
        {
            if (i == 0)
                
        }
    }
}