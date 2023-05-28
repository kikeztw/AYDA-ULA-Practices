#include <IOUtils.hpp>
#include <Variable.hpp>

std::ostream& operator << (std::ostream& out, const Variable& v)
{
  out << v.to_string();
  return out;
}

std::ostream& operator << (std::ostream& out, Variable* v)
{
  out << v->to_string();
  return out;
}
