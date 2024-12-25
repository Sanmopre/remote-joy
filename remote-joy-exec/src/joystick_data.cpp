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

std::string  parseJoysticks(const std::vector<JoystickDataStruct> &joysticks)
{
    JoysticksState joystickState;

    for (const auto [joystick, id] : joysticks)
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

    return joystickState.SerializeAsString();
}

void renderJoysticksInfo(const std::vector<SDL_Joystick *> &joysticks) {
    tabulate::Table joysticksTable;
    joysticksTable.add_row(tabulate::Table::Row_t{"Index", "Name", "Axis", "Buttons", "Directional pads", "Track balls",
                                                  "Vendor", "Product"});

    for (const auto &joystick: joysticks) {
        joysticksTable.add_row(tabulate::Table::Row_t{
                std::to_string(SDL_JoystickInstanceID(joystick)),
                SDL_JoystickName(joystick),
                std::to_string(SDL_JoystickNumAxes(joystick)),
                std::to_string(SDL_JoystickNumButtons(joystick)),
                std::to_string(SDL_JoystickNumHats(joystick)),
                std::to_string(SDL_JoystickNumBalls(joystick)),
                std::to_string(SDL_JoystickGetVendor(joystick)),
                std::to_string(SDL_JoystickGetProduct(joystick)),
        });
    }

    joysticksTable.column(1).format().font_align(tabulate::FontAlign::left);
    for (auto &column: joysticksTable[0]) {
        column.format()
                .font_color(tabulate::Color::yellow)
                .font_align(tabulate::FontAlign::center)
                .font_style({tabulate::FontStyle::bold});
    }

    std::cout << joysticksTable << std::endl;
}

std::optional<float> getAxisValue(const JoystickInput &joystick, uint8_t axis)
{
    if (axis >= joystick.axes_size()) {
        std::cout << " axis " << axis << " out of bounds" << std::endl;
        return std::nullopt;
    }

    return joystick.axes(axis);
}

std::optional<bool> getButtonPressedValue(const JoystickInput &joystick, uint8_t button)  {
    if (button >= joystick.buttons_size()) {
        std::cout << " button " << button << " out of bounds" << std::endl;
        return std::nullopt;
    }

    return joystick.buttons(button);
}

std::optional<JoystickInput> getJoystickInput(const JoysticksState &joysticks, uint8_t joystick)
{
    if (joystick >= joysticks.joysticks_size())
    {
        std::cout << " joystick " << joystick << " out of bounds" << std::endl;
        return std::nullopt;
    }

    return joysticks.joysticks(joystick);
}

} // namespace remote_joy
