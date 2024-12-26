#include "joystick-consumer.h"
namespace remote_joy {

// Constructor: Initializes IP and port
JoystickConsumer::JoystickConsumer(const std::string& remoteSenderIp, const std::string& receiveIp, uint16_t receivePort)
    : remoteSenderIp_(remoteSenderIp), receiveIp_(receiveIp), receivePort_(receivePort), socketFd_(-1)
{
#ifdef _WIN32
    // Initialize Winsock for Windows
    if (WSAStartup(MAKEWORD(2, 2), &wsaData_) != 0) {
        std::cerr << "WSAStartup failed with error " << WSAGetLastError() << std::endl;
        return;
    }
#endif

    // Create UDP socket (IPv4)
    socketFd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd_ < 0) {
        std::cerr << "Socket creation failed!" << std::endl;
        return;
    }

    // Set up the sockaddr_in structure for binding
    sockaddr_in receiveAddr {};
    receiveAddr.sin_family = AF_INET;
    receiveAddr.sin_port = htons(receivePort_);
    receiveAddr.sin_addr.s_addr = inet_addr(receiveIp_.c_str());

    // Bind the socket to the receive IP and port
    if (bind(socketFd_, reinterpret_cast<sockaddr*>(&receiveAddr), sizeof(receiveAddr)) < 0) {
        std::cerr << "Bind failed!" << std::endl;
        if (socketFd_ != -1) {
#ifdef _WIN32
            closesocket(socketFd_);
            WSACleanup();
#else
            close(socketFd_);
#endif
        }
        return;
    }

    // Start the receiving loop in a separate thread
    std::thread(&JoystickConsumer::receiveLoop, this).detach();
}

// Destructor: Cleans up resources
JoystickConsumer::~JoystickConsumer()
{
if (socketFd_ != -1) {
#ifdef _WIN32
    closesocket(socketFd_);
    WSACleanup();
#else
    close(socketFd_);
#endif
}
}

// Receives UDP packets and processes them
void JoystickConsumer::receiveLoop()
{
    char buffer[1024];  // Buffer to store received data
    sockaddr_in senderAddr {};
    socklen_t addrLen = sizeof(senderAddr);

    while (true) {
        // Receive UDP packet
        ssize_t receivedBytes = recvfrom(socketFd_, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&senderAddr), &addrLen);
        if (receivedBytes < 0) {
            std::cerr << "Error receiving data!" << std::endl;
            break;
        }

        // Process the received packet (this can be customized)
        std::cout << "Received packet from " << inet_ntoa(senderAddr.sin_addr) << ":" << ntohs(senderAddr.sin_port) << std::endl;
        std::cout << "Data: " << std::string(buffer, receivedBytes) << std::endl;

        // Simulate processing data (e.g., joystick axis/button parsing)
        // Example: if data is expected to be joystick state, you could parse here
    }
}

} // namespace remote_joy