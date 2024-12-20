#include "udp-sender.h"

// Platform-specific includes
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <iostream>

namespace remote_joy
{

UdpSender::UdpSender(const std::string &clientIp, uint16_t clientPort) :
    clientIp_(clientIp), clientPort_(clientPort) {

#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Failed to initialize Winsock\n";
        sock_ = INVALID_SOCKET;
        return;
    }
#endif

    sock_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_
#ifdef _WIN32
        == INVALID_SOCKET
#else
        < 0
#endif
    ) {
        std::cout << "Failed to create socket\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(clientPort_);
    if (inet_pton(AF_INET, clientIp.c_str(), &addr_.sin_addr) <= 0) {
        std::cout << "Invalid client IP address\n";
#ifdef _WIN32
        closesocket(sock_);
        WSACleanup();
#else
        close(sock_);
#endif
        sock_ =
#ifdef _WIN32
            INVALID_SOCKET;
#else
            -1;
#endif
        return;
    }
}

UdpSender::~UdpSender() {
    if (sock_
#ifdef _WIN32
        != INVALID_SOCKET
#else
        >= 0
#endif
    ) {
#ifdef _WIN32
        closesocket(sock_);
        WSACleanup();
#else
        close(sock_);
#endif
        std::cout << "Socket successfully closed\n";
    }
}

void UdpSender::send(const std::string &message) const
{
    if (sock_
#ifdef _WIN32
        == INVALID_SOCKET
#else
        < 0
#endif
    ) {
        std::cout << "Socket is not valid, cannot send message\n";
        return;
    }

    ssize_t sentBytes = sendto(sock_, message.c_str(), message.size(), 0,
                               (struct sockaddr*)&addr_, sizeof(addr_));
    if (sentBytes < 0)
    {
        std::cout << "Failed to send packet\n";
    }
}

} // namespace remote_joy