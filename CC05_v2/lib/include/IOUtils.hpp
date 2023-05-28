/**
 * @file IOUtils.hpp
 * @author Alejandro Mujica (aledrums@gmail.com)
 * @copyright Copyright (c) 2022
 */
#pragma once

#include <iomanip>
#include <iostream>
#include <memory>

#include <Models.hpp>

#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__); printf("\n");
#else
#define LOG(...)
#endif

std::ostream& operator << (std::ostream& out, const std::shared_ptr<Company>& c);

std::ostream& operator << (std::ostream& out, const std::shared_ptr<Plant>& p);

std::ostream& operator << (std::ostream& out, const std::shared_ptr<Good>& g);

std::ostream& operator << (std::ostream& out, const std::tuple<std::shared_ptr<Plant>, std::shared_ptr<Plant>, std::shared_ptr<Good>, float, float>& t);

std::ostream& operator << (std::ostream& out, const std::tuple<std::shared_ptr<Company>, std::shared_ptr<Company>, std::shared_ptr<Good>, float, float, float>& t);

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& v)
{
  out << v.size() << " elements\n";
  out << "[";
  auto it = v.begin();

  if (it == v.end())
  {
    out << "]";
    return out;
  }

  out << "\n  " << *it;
  ++it;

  for ( ; it != v.end(); ++it)
  {
    out << ",\n  " << *it;
  }

  out << "\n]";

  return out;
}
