#pragma once

// tabulate
#include "tabulate.hpp"

// protobuf
#include "joystick-data.pb.h"

// sdl2
#include <SDL_joystick.h>

#include "sdl-manager.h"

namespace remote_joy
{
std::string parseJoysticks(const std::vector<JoystickDataStruct>& joysticks);
void renderJoysticksInfo(const std::vector<SDL_Joystick*> &joysticks);
}