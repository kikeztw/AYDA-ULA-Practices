#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>
#include <tuple>

#include <sort.hpp>

#include <Definitions.hpp>

template <typename... T>
std::ostream& operator << (std::ostream& out, const std::tuple<T...>& t);

template <typename T>
std::ostream& operator << (std::ostream& out, const Designar::DynArray<T>& a);

template <typename T, size_t i, size_t size>
struct PrintTuple
{
  static void print(std::ostream& out, const T& t)
  {
    out << ", ";
    out << std::get<i>(t);
    PrintTuple<T, i + 1, size>::print(out, t);
  }
};

template <typename T, size_t size>
struct PrintTuple<T, 0, size>
{
  static void print(std::ostream& out, const T& t)
  {
    out << "(";
    out << std::get<0>(t);
    PrintTuple<T, 1, size>::print(out, t);
  }
};

template <typename T, size_t size>
struct PrintTuple<T, size, size>
{
  static void print(std::ostream& out, const T& t)
  {
    out << ")";
  }
};

template <typename... T>
std::ostream& operator << (std::ostream& out, const std::tuple<T...>& t)
{
  PrintTuple<std::tuple<T...>, 0, sizeof...(T)>::print(out, t);
  return out;
}

template <class Sequence, typename GetItem>
std::string sequence_to_string(const Sequence& s, const GetItem& get_item, bool multiline = true)
{
  if (s.is_empty())
  {
    return "[]";
  }

  std::stringstream ss;

  auto it = s.begin();

  ss << "[";
  if (multiline)
  {
    ss << "\n  ";
  }
  ss << get_item(*it);
  ++it;

  while (it != s.end())
  {
    if (multiline)
    {
      ss << ",\n  ";
    }
    else
    {
      ss << ", ";
    }
    ss << get_item(*it);
    ++it;
  }

  if (multiline)
  {
    ss << std::endl;
  }
  ss << "]";
  
  return ss.str();
}

template <class Node>
std::string block_to_string(const Designar::SLList<Node*>& block)
{
  if (block.is_empty())
  {
    return "[]";
  }

  std::stringstream ss;

  auto it = block.begin();

  ss << "  [\n    ";
  ss << (*it)->get_info();
  ++it;

  while (it != block.end())
  {
    ss << ",\n    " << (*it)->get_info();
    ++it;
  }

  ss << "\n  ]";
  
  return ss.str();
}

template <class Node>
std::string blocks_to_string(const Designar::SLList<Designar::SLList<Node*>>& blocks)
{
  if (blocks.is_empty())
  {
    return "[]";
  }

  std::stringstream ss;

  auto it = blocks.begin();

  ss << "[\n";
  ss << block_to_string(*it);
  ++it;

  while (it != blocks.end())
  {
    ss << ",\n" << block_to_string(*it);
    ++it;
  }

  ss << "\n]";
  
  return ss.str();  
}

template <typename T>
std::ostream& operator << (std::ostream& out, const Designar::DynArray<T>& a)
{
  out << sequence_to_string(a, [] (auto item) { return item; }, false);
  return out;
}

std::ostream& operator << (std::ostream& out, const Designar::SLList<CityMap::Node*>& nodes);

std::ostream& operator << (std::ostream& out, const Designar::SLList<RailwayMap::Node*>& nodes);

std::ostream& operator << (std::ostream& out, const Designar::SLList<Designar::SLList<CityMap::Node*>>& blocks);

std::ostream& operator << (std::ostream& out, const Designar::SLList<Designar::SLList<RailwayMap::Node*>>& blocks);

std::ostream& operator << (std::ostream& out, const Designar::SLList<CityMap::Arc*>& arcs);

std::ostream& operator << (std::ostream& out, const Designar::SLList<RailwayMap::Arc*>& arcs);

template <typename NodeInfo, typename ArcInfo>
std::ostream& operator << (std::ostream& out, const Designar::Graph<NodeInfo, ArcInfo>& map)
{
  auto nodes = Designar::sort(
    map.nodes(),
    [] (auto n1, auto n2)
    {
      return std::get<0>(n1->get_info()) < std::get<0>(n2->get_info());
    }
  );

  if (nodes.is_empty())
  {
    out << "Empty map";
    return out;
  }

  out << "Nodes (" << nodes.size() << ")\n";
  out << nodes << std::endl;

  auto arcs = Designar::sort(
    map.arcs(),
    [] (auto a1, auto a2)
    {
      if (std::get<0>(a1->get_src_node()->get_info()) == std::get<0>(a2->get_src_node()->get_info()))
      {
        return std::get<0>(a1->get_tgt_node()->get_info()) < std::get<0>(a2->get_tgt_node()->get_info());
      }
      return std::get<0>(a1->get_src_node()->get_info()) < std::get<0>(a2->get_src_node()->get_info());
    }
  );

  if (arcs.is_empty())
  {
    out << "Map without arcs";
    return out;
  }

  out << "Arcs (" << arcs.size() << ")\n";
  out << arcs;

  return out;
}

