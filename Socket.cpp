#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib> 
#include <cstring>

using namespace std;

/*
1) Create a socket (socket()).
2) Bind it to an IP/port (bind()).
3) Listen for incoming connections (listen()).
4) Accept a connection (accept()).
5) Communicate with the client (send()/recv()).
6) Close the client socket (close()).
*/

int main() 
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Define server address (IPv4, Port 8080, Any IP)
    struct sockaddr_in config{};             // Create struct & zero-initialize all members 
    config.sin_family = AF_INET;             // Select IPV4
    config.sin_addr.s_addr = INADDR_ANY;     // Listen on any IP
    config.sin_port = htons(8080);           // Convert port to network byte (big endian)
    

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        return (cerr << strerror(errno) << "\n", EXIT_FAILURE);


    if (bind(sockfd, (struct sockaddr*)&config, sizeof(config)) 
        || listen(sockfd, 5)) 
        return (close(sockfd), cerr << strerror(errno) << "\n", EXIT_FAILURE);
    
    cout << "Listening at port 8080\n";
    
    struct sockaddr_in client{};
    socklen_t client_len = sizeof(client);
    int client_sock = accept(sockfd, (struct sockaddr*)&client, &client_len);
    if (client_sock == -1)
        return (cerr << strerror(errno) << "\n", EXIT_FAILURE);
    
    cout << "Client connected!\n";
    
    int bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0) {
        cout << "Received from client: " << buffer << "\n";
    } else if (bytes_received == 0) {
        cout << "Client disconnected.\n";
    } else {
        cerr << "Receive failed: " << strerror(errno) << "\n";
    }

    // Step 6: Send "Connection success" message
    const char *message = "Connection success\n";
    send(client_sock, message, strlen(message), 0);

    // Step 7: Close the client socket
    close(client_sock);
    close(sockfd);
}
