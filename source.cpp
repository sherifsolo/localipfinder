#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

typedef unsigned char int8;

int main() {
    int8 iporHostnme[255];
    int hostnme;
    WSADATA wsaData;
    struct addrinfo hints = {}, * linkedAddrInfo = nullptr, * addrIntel = nullptr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        std::cerr << "[!?] WSAStartup() failed... error: " << WSAGetLastError() << std::endl;
        return -1;
    }
    std::cout << "[+] Initialized and running............... " << std::endl;

    // Get the hostname
    hostnme = gethostname((char*)iporHostnme, sizeof(iporHostnme));
    if (hostnme) {
        std::cerr << "[!?] gethostname() failed... error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }
    std::cout << "[+] Got hostname: " << iporHostnme << std::endl;

    // Configure hints for address resolution
    hints.ai_family = AF_UNSPEC;      // Allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_protocol = IPPROTO_TCP;

    // Get address info
    if (getaddrinfo((const char*)iporHostnme, nullptr, &hints, &linkedAddrInfo)) {
        std::cerr << "[!?] getaddrinfo() failed... error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }
    std::cout << "[+] getaddrinfo returned successfully............... \n\n" << std::endl;

    // Extract data from the addrinfo linked list
    int count = 0;
    for (addrIntel = linkedAddrInfo; addrIntel != nullptr; addrIntel = addrIntel->ai_next) {
        struct sockaddr_in* ipv4_addr = (struct sockaddr_in*)addrIntel->ai_addr;

        // Check address family
        if (addrIntel->ai_family == AF_INET) { // IPv4
            char ipstr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &ipv4_addr->sin_addr, ipstr, sizeof(ipstr));
            uint16_t port = ntohs(ipv4_addr->sin_port);
            std::cout << "IPv4 Address: " << ipstr << ", Port: " << port << std::endl;
        }
        else if (addrIntel->ai_family == AF_INET6) { // IPv6
            char ipstr[INET6_ADDRSTRLEN];
            struct sockaddr_in6* ipv6_addr = (struct sockaddr_in6*)addrIntel->ai_addr;
            inet_ntop(AF_INET6, &ipv6_addr->sin6_addr, ipstr, sizeof(ipstr));
            uint16_t port = ntohs(ipv6_addr->sin6_port);
            std::cout << "IPv6 Address: " << ipstr << ", Port: " << port << std::endl;
        }
        count++;
    }

    std::cout << "\n\n[+] Retrieved " << count << " addrinfo struct(s)." << std::endl;

    // Cleanup
    freeaddrinfo(linkedAddrInfo);
    WSACleanup();
    return 0;
}