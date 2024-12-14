// cli11
#include "CLI11.hpp"

// std
#include <string>

// protobuf
#include "joystick-data.pb.h"

// sdl2
#include <SDL2/SDL.h>

#include "joystick_data.h"


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
  std::vector<JoystickData> joysticks;

  int numJoysticks = SDL_NumJoysticks();
  std::cout << "Number of joysticks connected: " << numJoysticks << std::endl;

  for (int i = 0; i < numJoysticks; ++i) {
    JoystickData data;
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