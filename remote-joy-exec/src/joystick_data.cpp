#include "joystick_data.h"


namespace remote_joy
{

namespace
{
    [[nodiscard]] float normalizeAxis(const Sint16 axis)
    {
        return static_cast<float>(axis) / 32768.0f;
    }
}

void parseJoysticks(const std::vector<SDL_Joystick*> &joysticks, JoysticksState &joystickState)
{
    for (const auto joystick : joysticks)
    {
        auto* joystickInput = joystickState.add_joysticks();

        joystickInput->set_name(SDL_JoystickName(joystick));
        const auto numAxes = SDL_JoystickNumAxes(joystick);
        for (int i = 0; i < numAxes; ++i)
        {
            joystickInput->add_axes(normalizeAxis(SDL_JoystickGetAxis(joystick, i)));
        }

        const auto numButtons = SDL_JoystickNumButtons(joystick);
        for (int i = 0; i < numButtons; ++i)
        {
            joystickInput->add_buttons(SDL_JoystickGetButton(joystick, i));
        }
    }
}

void renderJoysticksInfo(const std::vector<SDL_Joystick*> &joysticks)
{
    tabulate::Table joysticksTable;
    joysticksTable.add_row(tabulate::Table::Row_t{"Index", "Name", "Axis", "Buttons", "Directional pads","Track balls", "Vendor", "Product"});

    for(const auto& joystick : joysticks)
    {
        joysticksTable.add_row(tabulate::Table::Row_t{std::to_string(SDL_JoystickInstanceID(joystick)),
          SDL_JoystickName(joystick), std::to_string(SDL_JoystickNumAxes(joystick)),
          std::to_string(SDL_JoystickNumButtons(joystick)),
            std::to_string(SDL_JoystickNumHats(joystick)),
            std::to_string(SDL_JoystickNumBalls(joystick)) ,
            std::to_string(SDL_JoystickGetVendor(joystick)),
            std::to_string(SDL_JoystickGetProduct(joystick)),
      });
    }

    joysticksTable.column(1).format().font_align(tabulate::FontAlign::left);
    for (auto & column : joysticksTable[0])
    {
        column
            .format()
            .font_color(tabulate::Color::yellow)
            .font_align(tabulate::FontAlign::center)
            .font_style({tabulate::FontStyle::bold});
    }

    std::cout << joysticksTable << std::endl;
}
}