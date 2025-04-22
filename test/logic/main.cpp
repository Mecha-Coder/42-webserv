#include "../include/header.hpp"

volatile sig_atomic_t stop = 0;

void stopServer(int sig)
{stop = 1;}

int main()
{
    int pollSize = 1;
    t_pollfd FD[MAX_CLIENTS];
    
    if (!initialize(FD))
        return EXIT_FAILURE;

    std::cout << "Server listening for request\n";
    signal(SIGINT, stopServer);    
    while (!stop)
    {
        if (!runServer(FD, pollSize)) 
            break;
    }

    std::cout << "Shutting down server!\n";
    for (int i = 0; i < pollSize; i++)
        close(FD[i].fd);
}
