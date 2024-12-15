#pragma once

// enet
#include "enet/enet.h"

// std
#include <string>
#include <iostream>

namespace remote_joy
{

class UdpConnector
{
protected:
    UdpConnector(const std::string& hostIp, uint16_t hostPort, const std::string& clientIp, uint16_t clientPort){};
    ~UdpConnector() = default;
};

class UdpSender final : public UdpConnector
{
public:
    UdpSender(const std::string& hostIp, uint16_t hostPort, const std::string& clientIp, uint16_t clientPort);
    ~UdpSender();
    void send(const std::string& message) const;

private:
    ENetHost* enetHost;
    ENetPeer* enetPeer;
};

class UdpReceiver : public UdpConnector {
public:
    UdpReceiver(const std::string& hostIp, uint16_t hostPort, const std::string& clientIp, uint16_t clientPort);
    ~UdpReceiver();

    void listen() const;

private:
    ENetHost* enetHost;
};
}