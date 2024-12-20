#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <cstdint>
#include <string>
#include <arpa/inet.h>
#endif

namespace remote_joy
{
    class UdpSender
    {
    public:
        UdpSender(const std::string& clientIp, uint16_t clientPort);
        ~UdpSender();

        void send(const std::string& message) const;

    private:
        const std::string clientIp_;
        const uint16_t clientPort_;
#ifdef _WIN32
        SOCKET sock_;
#else
        int sock_;
#endif
        sockaddr_in addr_;
    };

}
