#include "udp-connector.h"

namespace remote_joy
{

UdpSender::UdpSender(const std::string &hostIp, uint16_t hostPort, const std::string &clientIp, uint16_t clientPort)
    : UdpConnector(hostIp, hostPort, clientIp, clientPort), enetHost(nullptr), enetPeer(nullptr) {
    if (enet_initialize() != 0) {
        std::cout << "Failed to initialize ENet.\n";
        return;
    }

    // Bind the sender (host) to the specified IP and port
    ENetAddress address;
    enet_address_set_host(&address, hostIp.c_str());
    address.port = hostPort;

    enetHost = enet_host_create(&address, 1, 1, 0, 0); // Bind to the host's IP and port
    if (!enetHost) {
        std::cout << "Failed to create ENet host.\n";
        return;
    }

    // Set the peer's (receiver's) IP and port
    ENetAddress peerAddress;
    enet_address_set_host(&peerAddress, clientIp.c_str());
    peerAddress.port = clientPort;

    // Connect to the peer
    enetPeer = enet_host_connect(enetHost, &peerAddress, 1, 0);
    if (!enetPeer) {
        enet_host_destroy(enetHost);
        std::cout << "Failed to connect to the peer.\n";
        return;
    }

    // Wait for connection event
    ENetEvent event;
    if (enet_host_service(enetHost, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected to client at " << clientIp << ":" << clientPort << "\n";
    } else {
        std::cout << "Failed to connect to client within timeout.\n";
        enet_peer_reset(enetPeer);
        enet_host_destroy(enetHost);
        enetPeer = nullptr;
        enetHost = nullptr;
    }
}

UdpSender::~UdpSender()
{
    if (enetPeer) {
        enet_peer_disconnect(enetPeer, 0);
        enet_host_flush(enetHost);
    }
    if (enetHost) {
        enet_host_destroy(enetHost);
    }
    enet_deinitialize();
}

void UdpSender::send(const std::string &message) const {
    if (!enetPeer) {
        std::cout << "ENet peer is not connected.\n";
        return;
    }

    ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
    if (!packet) {
        std::cout << "Failed to create ENet packet.\n";
        return;
    }

    if (enet_peer_send(enetPeer, 0, packet) < 0) {
        std::cout << "Failed to send packet.\n";
        enet_packet_destroy(packet);
        return;
    }

    enet_host_flush(enetHost); // Ensure the packet is sent immediately.
    std::cout << "Packet sent: " << message << "\n";
}

}
