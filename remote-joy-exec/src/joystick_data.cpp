#include "joystick_data.h"


namespace remote_joy
{

namespace
{
    [[nodiscard]] float normalizeAxis(Sint16 axis)
    {
        return static_cast<float>(axis) / 32768.0f;
    }
}

void parseJoysticks(const std::vector<SDL_Joystick *> &joysticks, JoysticksState &joystickState)
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

// TODO: Make the input be std::vector<SDL_Joystick *> &joysticks since we can get all the needed info from there without the middle structure
void renderJoysticksInfo(std::vector<JoystickDisplayData> &joysticks)
{
    tabulate::Table joysticksTable;
    joysticksTable.add_row(tabulate::Table::Row_t{"Index", "Name", "Axis", "Buttons", "Directional pads","Track balls", "Vendor", "Product"});

    for(const auto& joystick : joysticks)
    {
        joysticksTable.add_row(tabulate::Table::Row_t{std::to_string(joystick.index),
          joystick.name, std::to_string(joystick.axisCount),
          std::to_string(joystick.buttonCount)
          ,  std::to_string(joystick.hatCount),
       std::to_string(joystick.ballCount) ,
    joystick.vendorID ,
        joystick.productID
      });
    }

    joysticksTable.column(1).format().font_align(tabulate::FontAlign::left);
    for (size_t i = 0; i < joysticksTable[0].size(); ++i)
    {
        joysticksTable[0][i]
            .format()
            .font_color(tabulate::Color::yellow)
            .font_align(tabulate::FontAlign::center)
            .font_style({tabulate::FontStyle::bold});
    }

    std::cout << joysticksTable << std::endl;
}
}