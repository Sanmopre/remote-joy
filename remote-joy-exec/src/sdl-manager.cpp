#include "sdl-manager.h"
#include "joystick_data.h"

// std
#include <iostream>

namespace remote_joy
{

SDLManager::SDLManager()
{
    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_JoystickEventState(SDL_ENABLE);


    while(SDL_NumJoysticks() == 0)
    {
        SDL_JoystickUpdate();
    }

    const int numJoysticks = SDL_NumJoysticks();

    std::vector<SDL_Joystick*> sdlJoysticks;
    sdlJoysticks.reserve(numJoysticks);

    for (int i = 0; i < numJoysticks; ++i)
    {
        // Open joystick
        SDL_Joystick* joystick = SDL_JoystickOpen(i);
        if (!joystick) {
            std::cout << "Failed to open joystick " << i << ": " << SDL_GetError() << std::endl;
            continue;
        }

        sdlJoysticks.emplace_back(joystick);

        // Close joystick
        //SDL_JoystickClose(joystick);
    }

    remote_joy::renderJoysticksInfo(sdlJoysticks);

}

SDLManager::~SDLManager()
{
    for(const auto& [joystick, joystickID] : joysticks_)
    {
        std::ignore = joystickID;
        SDL_JoystickClose(joystick);
    }
    joysticks_.clear();
    SDL_Quit();
}

void SDLManager::handleJoystickEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_JOYAXISMOTION:
                break;
            case SDL_JOYBUTTONDOWN:
                break;
            case SDL_JOYBUTTONUP:
                break;
            case SDL_JOYDEVICEADDED: {
                JoystickDataStruct joystickData {};
                joystickData.joystick = SDL_JoystickOpen(event.jdevice.which);
                joystickData.joystickID = event.jdevice.which;
                joysticks_.emplace_back(joystickData);
            }
                break;
            case SDL_JOYDEVICEREMOVED: {
                for(const auto& [joystick, joystickID] : joysticks_)
                {
                    if(joystickID == event.jdevice.which)
                    {
                        SDL_JoystickClose(joystick);
                    }
                }
            }
                break;
            default:
                break;
        }
    }
}

bool SDLManager::update() {
    handleJoystickEvents();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}
const std::vector<JoystickDataStruct> &SDLManager::getJoysticks() const
{
    return joysticks_;
}
} // namespace remote_joy
