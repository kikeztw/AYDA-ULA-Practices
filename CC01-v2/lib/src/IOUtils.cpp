#include <Color.hpp>
#include <IOUtils.hpp>

std::ostream& operator << (std::ostream& out, const Color& color)
{
  out << color.to_string();
  return out;
}
