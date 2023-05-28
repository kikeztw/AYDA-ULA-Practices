#include <Color.hpp>
#include <TestUtils.hpp>

bool operator == (const Color& c1, const Color& c2)
{
  return c1.get_red() == c2.get_red() && c1.get_green() == c2.get_green() && c1.get_blue() == c2.get_blue();
}

bool operator != (const Color& c1, const Color& c2)
{
  return !(c1 == c2);
}
