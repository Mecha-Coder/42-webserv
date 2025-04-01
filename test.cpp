#include <iostream>
#include <cstring>       // For memset
#include <unistd.h>      // For close
#include <sys/socket.h>  // For socket functions
#include <netinet/in.h>  // For sockaddr_in

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket (IPv4, TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to an IP and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept any incoming connection
    address.sin_port = htons(8080); // Port 8080

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listening on port 8080..." << std::endl;

    while (true) {
        // Accept incoming connection
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Read the request
        read(new_socket, buffer, 1024);
        std::cout << "Received request:\n" << buffer << std::endl;

        // Send HTTP response
        const char *http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, World!";
        send(new_socket, http_response, strlen(http_response), 0);

        // Close the connection
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
