#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    std::vector< std::pair<std::string,std::string> > listen_specs;
    listen_specs.push_back(std::make_pair(std::string("127.5.0.1"), std::string("65536")));
    listen_specs.push_back(std::make_pair(std::string("127.0.0.1"), std::string("9090")));

    std::vector<int> listeners;
    listeners.reserve(listen_specs.size());

    for (size_t i = 0; i < listen_specs.size(); ++i)
    {
        const std::string& host = listen_specs[i].first;
        const std::string& port = listen_specs[i].second; 

        struct addrinfo hints;
        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family   = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags    = AI_PASSIVE;

        struct addrinfo* result = NULL;
        int gai_err = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
        if (gai_err != 0) 
		{
            std::cerr << "getaddrinfo(\"" << host << "\", \"" << port
                      << "\") failed: " << gai_strerror(gai_err) << "\n";
            continue;  // skip to next host:port
        }

        int listen_fd = -1;
        struct addrinfo* rp = result;
        for (; rp != NULL; rp = rp->ai_next)
        {
            // Only AF_INET was requested, but we still check to be sure:
            if (rp->ai_family != AF_INET || rp->ai_socktype != SOCK_STREAM)
                continue;

            // 2.3) Create the socket
            listen_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (listen_fd < 0) {
                std::cerr << "  socket() failed: " << strerror(errno) << "\n";
                listen_fd = -1;
                continue; // try next addrinfo
            }

            // 2.4) Set SO_REUSEADDR so we can re‐bind quickly for testing.
            {
                int opt = 1;
                if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                    std::cerr << "  setsockopt(SO_REUSEADDR) failed: " << strerror(errno) << "\n";
                    close(listen_fd);
                    listen_fd = -1;
                    continue;
                }
            }

            // 2.5) Bind to the address/port
            if (bind(listen_fd, rp->ai_addr, rp->ai_addrlen) < 0) {
                std::cerr << "  bind() to " << host << ":" << port
                          << " failed: " << strerror(errno) << "\n";
                close(listen_fd);
                listen_fd = -1;
                continue;
            }

            // 2.6) Successful bind; break out of the loop
            break;
        }

        if (listen_fd < 0) {
            std::cerr << "Could not bind to " << host << ":" << port << ".\n";
            freeaddrinfo(result);
            continue;
        }

        // 2.7) Call listen() with a backlog. SOMAXCONN is the system max; you can pick 10 or 50 if you prefer.
        if (listen(listen_fd, SOMAXCONN) < 0) {
            std::cerr << "  listen() on " << host << ":" << port 
                      << " failed: " << strerror(errno) << "\n";
            close(listen_fd);
            freeaddrinfo(result);
            continue;
        }

        // 2.8) We now have a good listening socket. Print out which local address it bound to:
        {
            struct sockaddr_in bound_addr;
            socklen_t len = sizeof(bound_addr);
            if (getsockname(listen_fd, (struct sockaddr*)&bound_addr, &len) == 0) {
                char ipstr[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &bound_addr.sin_addr, ipstr, sizeof(ipstr));
                std::cout << "Listening on " << ipstr << ":" 
                          << ntohs(bound_addr.sin_port)
                          << " (fd=" << listen_fd << ")\n";
            } else {
                std::cerr << "  getsockname() failed: " << strerror(errno) << "\n";
            }
        }

        // 2.9) Save the listening socket for later I/O multiplexing
        listeners.push_back(listen_fd);

        // 2.10) Free the addrinfo, we don't need it anymore.
        freeaddrinfo(result);
    }

    if (listeners.empty()) {
        std::cerr << "No listening sockets were successfully created. Exiting.\n";
        return 1;
    }

    std::cout << "\n*** Example server is now listening on all specified loopback ports. ***\n";
    std::cout << "Press Ctrl+C to quit.\n";


    while (true) {
        pause();
    }


    for (size_t i = 0; i < listeners.size(); ++i) {
        close(listeners[i]);
    }

    return 0;
}
