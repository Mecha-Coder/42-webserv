#include <cstdio>    // for perror
#include <cerrno>    // for errno, EAGAIN, EWOULDBLOCK
#include <cstdlib>   // for exit()
#include <cstring>   // for memset (optional)
#include <unistd.h>  // for close, read, write
#include <fcntl.h>   // for fcntl
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <iostream>

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) { perror("socket"); return 1; }

    set_nonblocking(listen_fd);

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listen_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); return 1;
    }
    if (listen(listen_fd, SOMAXCONN) < 0) {
        perror("listen"); return 1;
    }

    std::vector<pollfd> fds;
    pollfd pfd;
    pfd.fd = listen_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    fds.push_back(pfd); 

    while (true) {
        int nready = poll(&fds[0], fds.size(), 1000);
        if (nready < 0) { perror("poll"); break; }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == listen_fd) {
                    sockaddr_in cli;
                    socklen_t len = sizeof(cli);
                    int cfd = accept(listen_fd, (sockaddr*)&cli, &len);
                    if (cfd >= 0) {
                        set_nonblocking(cfd);
                        pollfd pfd;
                        pfd.fd = listen_fd;
                        pfd.events = POLLIN;
                        pfd.revents = 0;
                        fds.push_back(pfd); 
                        std::cout << "New connection: " << cfd << "\n";
                    }
                } else {
                    char buf[512];
                    ssize_t n = recv(fds[i].fd, buf, sizeof(buf), 0);
                    if (n > 0) {
                        send(fds[i].fd, buf, n, 0);
                    } else if (n == 0 ||
                              (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) {
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i;
                    }
                }
            }
            if (fds[i].revents & (POLLERR|POLLHUP|POLLNVAL)) {
                close(fds[i].fd);
                fds.erase(fds.begin() + i);
                --i;
            }
        }
    }
    close(listen_fd);
    return 0;
}
