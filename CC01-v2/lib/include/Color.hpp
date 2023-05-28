#pragma once

#include <cstdint>
#include <string>

/// Class to represent a color by its red, green, and blue components.
class Color
{
public:
  Color(uint8_t r, uint8_t g, uint8_t b) noexcept;

  uint8_t get_red() const noexcept;

  uint8_t get_green() const noexcept;

  uint8_t get_blue() const noexcept;

  const std::string to_string() const noexcept;

private:
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};
