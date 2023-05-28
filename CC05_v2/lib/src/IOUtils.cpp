/**
 * @file IOUtils.cpp
 * @author Alejandro Mujica (aledrums@gmail.com)
 * @copyright Copyright (c) 2022
 */
#include <IOUtils.hpp>

std::ostream& operator << (std::ostream& out, const std::shared_ptr<Company>& c)
{
  out << "R.I.F. " << c->get_rif();
  return out;
}

std::ostream& operator << (std::ostream& out, const std::shared_ptr<Plant>& p)
{
  out << "Plant address: " << p.get() << " (Owner: " << p->get_owner()->get_rif() << ")";
  return out;
}

std::ostream& operator << (std::ostream& out, const std::shared_ptr<Good>& g)
{
  out << g->get_description();
  return out;
}

std::ostream& operator << (std::ostream& out, const std::tuple<std::shared_ptr<Plant>, std::shared_ptr<Plant>, std::shared_ptr<Good>, float, float>& t)
{
  out << "("
      << std::get<0>(t) << ", "
      << std::get<1>(t) << ", "
      << std::get<2>(t) << ", "
      << std::get<3>(t) << ", "
      << std::get<4>(t)
      << ")";
  return out;
}

std::ostream& operator << (std::ostream& out, const std::tuple<std::shared_ptr<Company>, std::shared_ptr<Company>, std::shared_ptr<Good>, float, float, float>& t)
{
  out << "("
      << std::get<0>(t) << ", "
      << std::get<1>(t) << ", "
      << std::get<2>(t) << ", "
      << std::get<3>(t) << ", "
      << std::get<4>(t) << ", "
      << std::get<5>(t)
      << ")";
  
  return out;
}