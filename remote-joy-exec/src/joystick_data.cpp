#include "joystick_data.h"

namespace remote_joy {
    void renderJoysticksInfo(std::vector<JoystickData> &joysticks)
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