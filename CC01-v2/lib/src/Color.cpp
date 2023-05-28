#include <sstream>

#include <Color.hpp>
  
Color::Color(uint8_t r, uint8_t g, uint8_t b) noexcept
  : red(r), green(g), blue(b)
{

}

uint8_t Color::get_red() const noexcept
{
  return red;
}

uint8_t Color::get_green() const noexcept
{
  return green;
}

uint8_t Color::get_blue() const noexcept
{
  return blue;
}

const std::string Color::to_string() const noexcept
{
  std::stringstream s;
  s << "Color(" << unsigned(red) << "," << unsigned(green) << "," << unsigned(blue) << ")";
  return s.str();
}