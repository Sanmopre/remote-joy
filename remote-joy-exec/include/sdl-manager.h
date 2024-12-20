#pragma once

// sdl
#include <SDL2/SDL.h>

// std
#include <vector>

namespace remote_joy
{

struct JoystickDataStruct
{
    SDL_Joystick* joystick;
    uint32_t joystickID;
};

class SDLManager
{
public:
    SDLManager();
    ~SDLManager();

    bool update();
    [[nodiscard]] const std::vector<JoystickDataStruct>& getJoysticks() const;

private:

    void handleJoystickEvents();
private:
    std::vector<JoystickDataStruct> joysticks_;
};

}