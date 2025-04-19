#include "../inc/web.hpp"

// #include <iostream>
// #include <sys/socket.h>
// #include <unistd.h>      // for close()
// #include <netinet/in.h>  // for sockaddr_in
// #include <arpa·/inet.h>   // for inet_pton(), htons()
// #include <cstring>     

int main()
{
    struct http_header* header;
    http_header* header = new http_header;
    memset(header, 0, sizeof(http_header));
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // int fd;
    void* recv_buffer = new char[1024];
    int flag = 1;
    size_t recv_len = 1024;
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
    recv(sockfd, recv_buffer, recv_len, flag);
    ft_parsing(recv_buffer, recv_len, header);
    // std::cout << "recv_buffer: " << static_cast<char*>(recv_buffer) << std::endl;
    return 0;
}
