#include <iostream>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

bool is_disallowed_ipv4(const struct sockaddr_in* sin)
{
    uint32_t ip = ntohl(sin->sin_addr.s_addr);
    //
    // Examples of checks:
    //  - Broadcast: 255.255.255.255  → 0xFFFFFFFF
    //  - “any”/unspecified: 0.0.0.0    → 0x00000000
    //  - You can also test for other reserved ranges if needed.
    //
    if (ip == 0xFFFFFFFFUL) return true;   // broadcast
    if (ip == 0x00000000UL) return true;   // unspecified/“any‐address”
    // (add other tests if you want to disallow more ranges)
    return false;
}

int main()
{
    struct addrinfo hint{}, *res;
    std::memset(&hint, 0, sizeof(hint));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags    = AI_NUMERICHOST | AI_NUMERICSERV;

    const char* host    = "255.255.255.255";
    const char* service = "5000";

    int err = getaddrinfo(host, service, &hint, &res);
    if (err != 0) {
        std::cout << "Failed: " << gai_strerror(err) << "\n";
        return 1;
    }

    // If getaddrinfo() “succeeds,” check if the address is disallowed:
    auto sin = reinterpret_cast<struct sockaddr_in*>(res->ai_addr);
    if (is_disallowed_ipv4(sin)) {
        std::cout << "Failed: disallowed broadcast/unspecified address\n";
        freeaddrinfo(res);
        return 1;
    }

    std::cout << "Passed (ip=" 
              << inet_ntoa(sin->sin_addr) 
              << ", port=" 
              << ntohs(sin->sin_port) 
              << ")\n";
    freeaddrinfo(res);
    return 0;
}
