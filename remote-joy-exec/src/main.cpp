// cli11
#include "CLI11.hpp"

// std
#include <string>

// protobuf
#include "joystick-data.pb.h"

// sdl2
#include <SDL2/SDL.h>

// enet
#include "enet/enet.h"

#include "joystick_data.h"

void handle_error(const std::string& error_message) {
  std::cerr << "Error: " << error_message << std::endl;
  exit(EXIT_FAILURE);
}

void client() {
  if (enet_initialize() != 0) {
    handle_error("ENet initialization failed");
  }

  ENetHost* client = enet_host_create(nullptr, 1, 2, 0, 0); // 1 outgoing connection
  if (!client) {
    handle_error("Client creation failed");
  }

  ENetAddress address;
  ENetPeer* peer;
  enet_address_set_host(&address, "127.0.0.1"); // Target server address
  address.port = 12345;                        // Target server port

  peer = enet_host_connect(client, &address, 2, 0); // Connect to the server
  if (!peer) {
    handle_error("Failed to initiate connection to server");
  }

  std::cout << "Connecting to server...\n";

  ENetEvent event;
  if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
    std::cout << "Connected to server.\n";

    // Send a message
    const char* message = "Hello, Server!";
    ENetPacket* packet = enet_packet_create(
        message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client); // Ensure the packet is sent

    std::cout << "Message sent to server.\n";
  } else {
    handle_error("Failed to connect to server");
  }

  enet_peer_disconnect(peer, 0);
  enet_host_destroy(client);
  enet_deinitialize();
}

int main(int argc, char **argv)
{
  CLI::App app("Joystick UDP data sender.");
  CLI11_PARSE(app, argc, argv);

  if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
  {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_JoystickEventState(SDL_ENABLE);
  std::vector<remote_joy::JoystickData> joysticks;

  int numJoysticks = SDL_NumJoysticks();
  std::cout << "Number of joysticks connected: " << numJoysticks << std::endl;

  for (int i = 0; i < numJoysticks; ++i) {
    remote_joy::JoystickData data;
    data.index = i;

    // Open joystick
    SDL_Joystick* joystick = SDL_JoystickOpen(i);
    if (!joystick) {
      std::cerr << "Failed to open joystick " << i << ": " << SDL_GetError() << std::endl;
      continue;
    }

    // Retrieve information
    data.name = SDL_JoystickName(joystick) ? SDL_JoystickName(joystick) : "Unknown";
    data.buttonCount = SDL_JoystickNumButtons(joystick);
    data.axisCount = SDL_JoystickNumAxes(joystick);
    data.hatCount = SDL_JoystickNumHats(joystick);
    data.ballCount = SDL_JoystickNumBalls(joystick);

    // Get Vendor and Product IDs (if supported)
    SDL_JoystickGUID guid = SDL_JoystickGetGUID(joystick);
    char guidString[33];
    SDL_JoystickGetGUIDString(guid, guidString, sizeof(guidString));
    data.vendorID = guidString;  // Use GUID as a general identifier

    // Add to list
    joysticks.push_back(data);

    // Close joystick
    SDL_JoystickClose(joystick);
  }

  renderJoysticksInfo(joysticks);

  SDL_Quit();

  return 0;
}