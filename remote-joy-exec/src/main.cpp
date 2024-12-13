// cli11
#include "CLI11.hpp"

// std
#include <string>

// protobuf
#include "joystick-data.pb.h"

// sdl2
#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
  CLI::App app("Joystick UDP data sender.");
  CLI11_PARSE(app, argc, argv);

  if (SDL_Init(SDL_INIT_JOYSTICK) != 0)
  {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_Quit();

  return 0;
}