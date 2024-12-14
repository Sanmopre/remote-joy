#pragma once

// std
#include <string>

// tabulate
#include "tabulate.hpp"

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


inline void renderJoysticksInfo(std::vector<JoystickData>& joysticks)
{
  tabulate::Table joysticksTable;
  joysticksTable.add_row(tabulate::Table::Row_t{"Index", "Name", "Axis", "Buttons", "Directional pads","Track balls", "Vendor", "Product"});

  for(const auto& joystick : joysticks) {
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