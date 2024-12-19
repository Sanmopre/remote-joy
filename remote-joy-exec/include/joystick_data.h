#pragma once

// std
#include <string>

// tabulate
#include "tabulate.hpp"

// protobuf
#include "joystick-data.pb.h"

// sdl2
#include <SDL_joystick.h>

namespace remote_joy
{

// TODO: Remove this unnecesary structure
struct JoystickDisplayData
{
  uint8_t index;
  std::string name;
  uint8_t buttonCount;
  uint8_t axisCount;
  uint8_t hatCount;        // Number of directional pads
  uint8_t ballCount;       // Number of trackballs
  std::string vendorID;    // Vendor ID (if available)
  std::string productID;   // Product ID (if available)
};

void parseJoysticks(const std::vector<SDL_Joystick*>& joysticks, JoysticksState& joystickState);

// TODO: Change signature
void renderJoysticksInfo(std::vector<JoystickDisplayData>& joysticks);

}