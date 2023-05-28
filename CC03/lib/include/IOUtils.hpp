#pragma once

#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__); printf("\n");
#else
#define LOG(...)
#endif

#include <iostream>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

class Variable;

std::ostream& operator << (std::ostream& out, const Variable& v);

std::ostream& operator << (std::ostream& out, Variable* v);

template <typename First, typename Second>
std::ostream& operator << (std::ostream& out, const std::pair<First, Second>& p);

template <typename First, typename Second>
std::ostream& operator << (std::ostream& out, const std::pair<First*, Second*>& p);

template <typename T>
std::ostream& operator << (std::ostream& out, const std::optional<T>& opt)
{
  if (opt.has_value())
  {
    out << opt.value();
  }
  else
  {
    out << "nullopt";
  } 
  return out;
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& p);

template <typename First, typename Second>
std::ostream& operator << (std::ostream& out, const std::pair<First, Second>& p)
{
  out << "(" << p.first << ", " << p.second << ")";
  return out;
}

template <typename First, typename Second>
std::ostream& operator << (std::ostream& out, const std::pair<First*, Second*>& p)
{
  out << "(" << *p.first << ", " << *p.second << ")";
  return out;
}

template <typename C>
void print_container(const C& c, char ldelim, char rdelim, std::ostream& out)
{
  out << ldelim;
  auto it = c.begin();

  if (it != c.end())
  {
    out << *it;
    ++it;
  }

  for ( ; it != c.end(); ++it)
  {
    out << ", " << *it;
  }

  out << rdelim;
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& v)
{
  print_container(v, '[', ']', out);
  return out;
}

template <typename T, class H = std::hash<T>>
std::ostream& operator << (std::ostream& out, const std::unordered_set<T, H>& s)
{
  print_container(s, '{', '}', out);
  return out;
}

template <typename K, typename V, class H = std::hash<K>, class KEq = std::equal_to<K>>
std::ostream& operator << (std::ostream& out, const std::unordered_map<K, V, H, KEq>& m)
{
  out << "{";
  auto it = m.begin();

  if (it == m.end())
  {
    out << "}";
    return out;
  }

  out << "\n  " << it->first << ": " << it->second;
  ++it;
  
  for ( ; it != m.end(); ++it)
  {
    out << ",\n  " << it->first << ": " << it->second;
  }

  out << "\n}";
  return out;
}

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<std::vector<T>>& m)
{
  out << "[";
  auto it = m.begin();

  if (it == m.end())
  {
    out << "]";
    return out;
  }

  out << *it;
  ++it;
  
  for ( ; it != m.end(); ++it)
  {
    out << ",\n " << *it;
  }

  out << "]";

  return out;
}
