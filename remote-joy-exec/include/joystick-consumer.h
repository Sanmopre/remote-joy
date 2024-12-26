#pragma once

// std
#include <cstdint>
#include <memory>
#include <string>
#include <mutex>


// Platform-specific includes
#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

class JoystickInput;

namespace remote_joy
{

class JoystickConsumer
{
public:
    JoystickConsumer(const std::string& remoteSenderIp, const std::string& receiveIp, uint16_t receivePort);
    ~JoystickConsumer();

private:
    void receiveLoop();

private:
    // udp commmunications variables
    const std::string remoteSenderIp_;
    const std::string receiveIp_;
    const uint16_t receivePort_;
    int socketFd_;
#ifdef _WIN32
    WSADATA wsaData_;
#endif

    std::shared_ptr<JoystickInput> joystickInput_;
    std::mutex mutex_;
    const size_t bufferSize = 2048;
};

} // namespace remote_joy