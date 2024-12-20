// cli11
#include "CLI11.hpp"

// std
#include <string>

// sdl2
#include <SDL2/SDL.h>

#include "sdl-manager.h"
#include "udp-sender.h"
#include "joystick_data.h"

// TODO: Clean main and handle SDL joysticks by events not in that while loop

int main(int argc, char **argv)
{
  CLI::App app("Joystick UDP data sender.");
  CLI11_PARSE(app, argc, argv);

  remote_joy::SDLManager sdlManager;
  const remote_joy::UdpSender udpSender {"127.0.0.1", 7799};

  bool running = true;
  while(running)
  {
    running = sdlManager.update();
    udpSender.send(remote_joy::parseJoysticks(sdlManager.getJoysticks()));
  }

  return 0;
}