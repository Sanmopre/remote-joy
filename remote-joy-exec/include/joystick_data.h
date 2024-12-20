#pragma once

// tabulate
#include "tabulate.hpp"

// protobuf
#include "joystick-data.pb.h"

// sdl2
#include <SDL_joystick.h>

namespace remote_joy
{
void parseJoysticks(const std::vector<SDL_Joystick*>& joysticks, JoysticksState& joystickState);

void renderJoysticksInfo(const std::vector<SDL_Joystick*> &joysticks);
}