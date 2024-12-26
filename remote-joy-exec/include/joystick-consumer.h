#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <thread>
#include <cstring>

// Platform-specific includes
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

namespace remote_joy {

class JoystickConsumer
{
public:
    JoystickConsumer(const std::string& remoteSenderIp, const std::string& receiveIp, uint16_t receivePort);
    ~JoystickConsumer();

private:
    void receiveLoop();

private:
    const std::string remoteSenderIp_;
    const std::string receiveIp_;
    const uint16_t receivePort_;
    int socketFd_;

#ifdef _WIN32
    WSADATA wsaData_;
#endif
};

} // namespace remote_joy