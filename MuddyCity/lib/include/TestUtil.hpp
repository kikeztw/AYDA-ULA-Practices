#pragma once

#include <math.hpp>

#include <Definitions.hpp>

// Basic compare
template <typename T>
bool equal_value(const T& v1, const T& v2)
{
  return v1 == v2;
}

// Float compare
template <>
bool equal_value<float>(const float& f1, const float& f2);

bool equal_value(const Designar::DynArray<Obstacle>& a1, const Designar::DynArray<Obstacle>& a2);

// Tuple compare
template<typename T, size_t i, size_t size>
struct TupleCompare
{
  static constexpr bool eq(const T& t1, const T& t2)
  {
	  return equal_value(std::get<i>(t1), std::get<i>(t2))
      && TupleCompare<T, i + 1, size>::eq(t1, t2);
  }
};

template<typename T, size_t size>
struct TupleCompare<T, size, size>
{
  static constexpr bool eq(const T&, const T&) { return true; }
};

template<typename... T>
bool equal_value(const std::tuple<T...>& t1, const std::tuple<T...>& t2)
{
  return TupleCompare<std::tuple<T...>, 0, sizeof...(T)>::eq(t1, t2);
}

bool equal_value(CityMap::Arc* a1, CityMap::Arc* a2);

bool equal_value(RailwayMap::Arc* a1, RailwayMap::Arc* a2);

template <class NodeInfo, class ArcInfo>
bool equal_value(const Designar::Graph<NodeInfo, ArcInfo>& g1, const Designar::Graph<NodeInfo, ArcInfo>& g2)
{
  auto node_cmp = [] (auto n1, auto n2)
  {
    return std::get<0>(n1->get_info()) < std::get<0>(n2->get_info());
  };

  auto nodes1 = sort(g1.nodes(), node_cmp);
  auto nodes2 = sort(g2.nodes(), node_cmp);
  
  if (!nodes1.equal(nodes2, [] (auto n1, auto n2) { return equal_value(n1->get_info(), n2->get_info()); }))
  {
    return false;
  }

  auto arc_cmp = [] (auto a1, auto a2)
  {
    if (std::get<0>(a1->get_src_node()->get_info()) == std::get<0>(a2->get_src_node()->get_info()))
    {
      return std::get<0>(a1->get_tgt_node()->get_info()) < std::get<0>(a2->get_tgt_node()->get_info());
    }
    return std::get<0>(a1->get_src_node()->get_info()) < std::get<0>(a2->get_src_node()->get_info());
  };

  auto arcs1 = sort(g1.arcs(), arc_cmp);
  auto arcs2 = sort(g2.arcs(), arc_cmp);

  return arcs1.equal(arcs2, [] (auto a1, auto a2) { return equal_value(a1, a2); });
}

template <class Node>
bool equal_value(const Designar::SLList<Node*>& block1, const Designar::SLList<Node*>& block2)
{
  return block1.equal(block2, [] (auto n1, auto n2) { return equal_value(n1->get_info(), n2->get_info()); });
}

template <class Node>
bool equal_value(const Designar::SLList<Designar::SLList<Node*>>& blocks1, const Designar::SLList<Designar::SLList<Node*>>& blocks2)
{
  return blocks1.equal(blocks2, [] (const auto& b1, const auto& b2) { return equal_value(b1, b2); });
}

template <typename T>
void assert_equal(const T& current, const T& expected)
{
  if (!equal_value(expected, current))
  {
    std::cerr << "FAILED:\n";
    std::cerr << "Current: " << current << std::endl;
    std::cerr << "Expected: " << expected << std::endl;
    abort();
  }
  std::cout << "OK.\n";
}

template <class Exception, class Method, typename ...Args>
void assert_throw_exception(const Method& method, Args&&... args)
{
  try
  {
    method(args...);
    std::cerr << "FAILED:\n";
    std::cerr << "No exception thrown\n";
  }
  catch (Exception)
  {
    std::cout << "OK.\n";
    return;
  }
  catch (...)
  {
    std::cerr << "FAILED:\n";
    std::cerr << "Another exception type was thrown\n";
  }

  abort();
}
