#include <IOUtil.hpp>

std::ostream& operator << (std::ostream& out, const Designar::SLList<CityMap::Node*>& nodes)
{
  out << sequence_to_string(nodes, [] (auto n) { return n->get_info(); });
  return out;
}

std::ostream& operator << (std::ostream& out, const Designar::SLList<RailwayMap::Node*>& nodes)
{
  out << sequence_to_string(nodes, [] (auto n) { return n->get_info(); });
  return out;
}

std::ostream& operator << (std::ostream& out, const Designar::SLList<Designar::SLList<CityMap::Node*>>& blocks)
{
  out << blocks_to_string(blocks);
  return out;
}

std::ostream& operator << (std::ostream& out, const Designar::SLList<Designar::SLList<RailwayMap::Node*>>& blocks)
{
  out << blocks_to_string(blocks);
  return out;
}

std::ostream& operator << (std::ostream& out, const Designar::SLList<CityMap::Arc*>& arcs)
{
  out << sequence_to_string(
    arcs,
    [] (auto a)
    {
      std::stringstream ss;
      ss << a->get_src_node()->get_info() << " -- " << a->get_tgt_node()->get_info();
      return ss.str();

    }
  );
  return out;
}

std::ostream& operator << (std::ostream& out, const Designar::SLList<RailwayMap::Arc*>& arcs)
{
  out << sequence_to_string(
    arcs,
    [] (auto a)
    {
      std::stringstream ss;
      ss << a->get_src_node()->get_info() << " -- " << a->get_tgt_node()->get_info() << ": " << a->get_info();
      return ss.str();
    }
  );
  return out;
}