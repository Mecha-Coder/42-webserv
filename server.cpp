/* 
 * Non-blocking HTTP server in C++98
 * Handles 10k+ simultaneous connections using poll()
 * Compliant with HTTP/1.1 keep-alive requirements
 */

// Standard C/C++ headers
#include <vector>
#include <map>
#include <poll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <string>

// Network headers
#include <netinet/in.h>
#include <arpa/inet.h>

// Configuration constants
#define MAX_CLIENTS 10000     // Maximum simultaneous connections
#define BUFFER_SIZE 4096      // Per-client read/write buffer size
#define TIMEOUT_MS 30000      // poll() timeout in milliseconds

// Client connection state structure
struct Client 
{
    int fd;                   // Socket file descriptor
    char read_buf[BUFFER_SIZE]; // Buffer for incoming data
    size_t read_bytes;        // Bytes received in current request
    char write_buf[BUFFER_SIZE]; // Buffer for outgoing data
    size_t write_bytes;       // Total bytes to send
    size_t write_sent;        // Bytes already sent
    bool closing;             // Flag to mark for cleanup

    Client(int fd) : fd(fd), read_bytes(0), write_bytes(0), 
                    write_sent(0), closing(false) {}
};

// Global state management
std::vector<struct pollfd> poll_fds;     // All monitored file descriptors
std::map<int, Client*> clients;          // Active clients: fd -> Client*
std::map<int, size_t> fd_to_index;       // fd -> index in poll_fds vector

/* Set socket to non-blocking mode */
void set_nonblocking(int fd)
{
    // Get current flags
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }
    
    // Add non-blocking flag
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

/* Initialize listening socket */
void init_server(int port) 
{
    // Create TCP socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Allow socket reuse to avoid TIME_WAIT issues
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Set non-blocking mode for accept()
    set_nonblocking(listen_fd);

    // Bind to all interfaces on specified port
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Start listening with system-defined backlog
    if (listen(listen_fd, SOMAXCONN))
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Add listening socket to poll set
    struct pollfd pfd;
    pfd.fd = listen_fd;
    pfd.events = POLLIN;  // Monitor for incoming connections
    poll_fds.push_back(pfd);
}

/* Accept new client connections */
void accept_clients(int listen_fd)
{
    // Process all pending connections
    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addrlen);
        
        // Handle accept() results
        if (client_fd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No more pending connections
                break;
            }
            perror("accept");
            continue;
        }

        // Reject if server at capacity
        if (clients.size() >= MAX_CLIENTS) {
            close(client_fd);
            continue;
        }

        // Configure client socket
        set_nonblocking(client_fd);
        
        // Add to poll monitoring
        struct pollfd pfd;
        pfd.fd = client_fd;
        pfd.events = POLLIN;  // Initial interest: read availability
        poll_fds.push_back(pfd);
        
        // Create client state
        clients[client_fd] = new Client(client_fd);
        fd_to_index[client_fd] = poll_fds.size() - 1;  // Track position
    }
}

/* Handle incoming data from client */
void handle_read(Client* client) {
    // Read available data into buffer
    ssize_t bytes = recv(client->fd, 
                        client->read_buf + client->read_bytes,
                        BUFFER_SIZE - client->read_bytes, 0);

    if (bytes > 0) {
        client->read_bytes += bytes;
        
        // Detect HTTP header end (double CRLF)
        if (memmem(client->read_buf, client->read_bytes, "\r\n\r\n", 4)) {
            // Prepare HTTP response
            const char* response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 13\r\n"
                "Connection: keep-alive\r\n\r\n"
                "Hello, World!";
            size_t len = strlen(response);
            memcpy(client->write_buf, response, len);
            client->write_bytes = len;
            
            // Switch to write monitoring
            size_t idx = fd_to_index[client->fd];
            poll_fds[idx].events = POLLOUT;
        }
    }
    else if (bytes == 0 || (bytes == -1 && errno != EAGAIN)) {
        // Connection closed or error
        client->closing = true;
    }
}

/* Handle outgoing data to client */
void handle_write(Client* client) {
    // Send buffered data
    ssize_t bytes = send(client->fd, 
                       client->write_buf + client->write_sent,
                       client->write_bytes - client->write_sent, 0);

    if (bytes > 0) {
        client->write_sent += bytes;
        
        // Check if full response sent
        if (client->write_sent >= client->write_bytes) {
            // Reset for next request
            client->read_bytes = 0;
            client->write_bytes = 0;
            client->write_sent = 0;
            
            // Switch back to read monitoring
            size_t idx = fd_to_index[client->fd];
            poll_fds[idx].events = POLLIN;
        }
    }
    else if (bytes == -1 && errno != EAGAIN) {
        client->closing = true;
    }
}

/* Clean up client resources */
void cleanup_client(int fd) {
    // Find client's position in poll_fds
    std::map<int, size_t>::iterator idx_it = fd_to_index.find(fd);
    if (idx_it == fd_to_index.end()) return;
    
    // Swap with last element for O(1) removal
    size_t idx = idx_it->second;
    if (idx != poll_fds.size() - 1) {
        std::swap(poll_fds[idx], poll_fds.back());
        fd_to_index[poll_fds[idx].fd] = idx;
    }
    
    // Remove from tracking structures
    poll_fds.pop_back();
    fd_to_index.erase(fd);
    
    // Free resources
    delete clients[fd];
    clients.erase(fd);
    close(fd);
}

/* Main server loop */
int main() {
    // Initialize listening socket
    init_server(8080);

    // Event loop
    while (true) 
    {
        // Wait for events
        int ready = poll(&poll_fds[0], poll_fds.size(), TIMEOUT_MS);
        if (ready == -1) 
        {
            if (errno == EINTR) continue;  // Interrupted by signal
            perror("poll");
            exit(EXIT_FAILURE);
        }

        // Handle new connections
        if (poll_fds[0].revents & POLLIN)
            accept_clients(poll_fds[0].fd);

        // Process client events
        for (size_t i = 1; i < poll_fds.size(); ++i)
        {
            if (poll_fds[i].revents == 0) continue;
            
            int fd = poll_fds[i].fd;
            Client* client = clients[fd];

            // Handle errors
            if (poll_fds[i].revents & (POLLERR | POLLHUP)) {
                client->closing = true;
            }
            else {
                // Handle read/write events
                if (poll_fds[i].revents & POLLIN) handle_read(client);
                if (poll_fds[i].revents & POLLOUT) handle_write(client);
            }

            // Cleanup if needed
            if (client->closing) {
                cleanup_client(fd);
                --i;  // Adjust index after removal
            }
        }
    }

    return 0;
}