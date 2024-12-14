#pragma once

// std
#include <string>

// tabulate
#include "tabulate.hpp"

namespace remote_joy
{

struct JoystickData
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


void renderJoysticksInfo(std::vector<JoystickData>& joysticks);

}