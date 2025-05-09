#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <string>
#include <vector>
#include <stdio.h>

#define PORT 8080
#define MAX_CLIENTS 1024       // Increased from 64
#define BUFFER_SIZE 4096
#define MAX_REQUEST_SIZE 16384  // 16KB max request buffer

volatile sig_atomic_t stop_flag = 0;

struct Client {
    int fd;
    std::string buffer;
};

void handle_signal(int sig) {
    stop_flag = 1;
}

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, nullptr);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket"); 
        return EXIT_FAILURE;
    }

    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(listen_fd);
        return EXIT_FAILURE;
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(listen_fd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(listen_fd);
        return EXIT_FAILURE;
    }

    if (listen(listen_fd, SOMAXCONN) == -1) {
        perror("listen");
        close(listen_fd);
        return EXIT_FAILURE;
    }

    if (set_nonblocking(listen_fd) == -1) {
        perror("set_nonblocking (listen)");
        close(listen_fd);
        return EXIT_FAILURE;
    }

    std::vector<Client> clients;
    struct pollfd fds[MAX_CLIENTS];
    memset(fds, 0, sizeof(fds));
    
    // Add listening socket
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;
    int nfds = 1;

    printf("Server listening on port %d\n", PORT);

    while (!stop_flag) 
    {
        int ready = poll(fds, nfds, 1000);

        if (ready == -1) 
        {
            if (errno == EINTR) continue;
            perror("poll");
            break;
        }

        if (ready == 0) continue;

        for (int i = 0; i < nfds; ++i) 
        {
            if (fds[i].revents == 0) continue;

            // Handle errors and hangups
            if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) 
            {
                printf("Closing bad fd %d\n", fds[i].fd);
                close(fds[i].fd);
                fds[i] = fds[--nfds];
                i--;
                continue;
            }

            if (fds[i].revents & POLLIN) 
            {
                if (fds[i].fd == listen_fd) 
                {
                    // Accept new connections
                    while (nfds < MAX_CLIENTS) 
                    {
                        int client_fd = accept(listen_fd, nullptr, nullptr);
                        if (client_fd == -1) 
                        {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                            perror("accept");
                            break;
                        }

                        if (set_nonblocking(client_fd) == -1)
                        {
                            perror("set_nonblocking (client)");
                            close(client_fd);
                            continue;
                        }

                        // Add to pollfd array
                        fds[nfds].fd = client_fd;
                        fds[nfds].events = POLLIN;
                        clients.push_back({client_fd, ""});
                        nfds++;
                        printf("Accepted new connection (fd: %d)\n", client_fd);
                    }
                } 
                
                else 
                {
                    // Handle client data with partial reads
                    char buf[BUFFER_SIZE];
                    ssize_t bytes_read;
                    Client* client = nullptr;

                    // Find client in vector
                    for (auto& c : clients) 
                    {
                        if (c.fd == fds[i].fd) 
                        {
                            client = &c;
                            break;
                        }
                    }

                    if (!client) 
                    {
                        close(fds[i].fd);
                        fds[i] = fds[--nfds];
                        i--;
                        continue;
                    }

                    // Read loop for partial data
                    while ((bytes_read = read(fds[i].fd, buf, sizeof(buf))))
                    {
                        if (bytes_read == -1) {
                            if (errno == EAGAIN) break;  // No more data
                            perror("read");
                            break;
                        }

                        if (bytes_read == 0) {
                            printf("Client closed connection (fd: %d)\n", fds[i].fd);
                            break;
                        }

                        // Append to client buffer
                        client->buffer.append(buf, bytes_read);

                        // Prevent buffer overflow
                        if (client->buffer.size() > MAX_REQUEST_SIZE) {
                            printf("Request too large (fd: %d)\n", fds[i].fd);
                            break;
                        }
                    }

                    // Check if we should process the request
                    if (bytes_read <= 0 || client->buffer.size() >= MAX_REQUEST_SIZE) {
                        // Send response even if incomplete
                        const char* resp = 
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-Length: 13\r\n\r\n"
                            "Hello, world!";
                        
                        size_t total_sent = 0;
                        size_t resp_len = strlen(resp);
                        
                        while (total_sent < resp_len) {
                            ssize_t bytes_sent = write(fds[i].fd, 
                                                     resp + total_sent, 
                                                     resp_len - total_sent);
                            if (bytes_sent == -1) {
                                if (errno == EAGAIN) continue;
                                perror("write");
                                break;
                            }
                            total_sent += bytes_sent;
                        }

                        // Cleanup
                        close(fds[i].fd);
                        fds[i] = fds[--nfds];
                        i--;
                        clients.erase(std::remove_if(clients.begin(), clients.end(),
                            [&](const Client& c) { return c.fd == fds[i].fd; }), 
                            clients.end());
                    }
                }
            }
        }
    }

    // Cleanup
    printf("\nShutting down...\n");
    for (int i = 0; i < nfds; ++i) {
        close(fds[i].fd);
    }
    close(listen_fd);

    return EXIT_SUCCESS;
}