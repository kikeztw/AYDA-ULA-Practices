#include <TestUtil.hpp>

template <>
bool equal_value<float>(const float& f1, const float& f2)
{
  return Designar::real_equal(f1, f2);
}

bool equal_value(const Designar::DynArray<Obstacle>& a1, const Designar::DynArray<Obstacle>& a2)
{
  return a1.equal(a2, [] (const auto& item1, const auto& item2) { return equal_value(item1, item2); });
}

bool equal_value(CityMap::Arc* a1, CityMap::Arc* a2)
{
  return equal_value(a1->get_src_node()->get_info(), a2->get_src_node()->get_info()) &&
    equal_value(a1->get_tgt_node()->get_info(), a2->get_tgt_node()->get_info());
}

bool equal_value(RailwayMap::Arc* a1, RailwayMap::Arc* a2)
{
  return equal_value(a1->get_src_node()->get_info(), a2->get_src_node()->get_info()) &&
    equal_value(a1->get_tgt_node()->get_info(), a2->get_tgt_node()->get_info()) &&
    equal_value(a1->get_info(), a2->get_info());
}
