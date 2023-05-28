/**
 * @file ProductionNetwork.cpp
 * @authors
 */
#include <ProductionNetwork.hpp>
#include <vector>
#include <map>

void DirectedGraph::iterate_nodes(std::function<void(Node)> operation) const {
  for (const auto& node : nodes) {
    operation(node);
  }
}

void DirectedGraph::iterate_arcs(std::function<void(Arc)> operation) const {
  for (const auto& arc : arcs) {
    operation(arc);
  }
}

void DirectedGraph::insert_node(Node node) {
    nodes.push_back(node);
}

void DirectedGraph::insert_arc(Arc arc) {
    arcs.push_back(arc);
}

DirectedGraph::Node DirectedGraph::get_root_node() const {
    std::vector<Node> destinations;
    for (const auto& arc : arcs) {
        destinations.push_back(std::get<1>(arc));
    }
    for (const auto& node : nodes) {
        if (std::find(destinations.begin(), destinations.end(), node) == destinations.end()) {
            return node;
        }
    }
    return nullptr;
}

std::vector<std::pair<DirectedGraph::Node, std::shared_ptr<Good>>> DirectedGraph::get_first_level_nodes() const {
    std::vector<std::pair<DirectedGraph::Node, std::shared_ptr<Good>>> firstLevelNodes;
    auto rootNode = get_root_node();
    if (rootNode == nullptr) {
      return firstLevelNodes;
    }
    for (const auto& arc : arcs) {
      if (std::get<0>(arc) == rootNode) {
        std::cout<<std::get<2>(arc)<<"\n";
        firstLevelNodes.push_back(std::make_pair(std::get<1>(arc), std::get<2>(arc)));
      }
    }
    return firstLevelNodes;
}

ProductionNetwork::ProductionNetwork(const ProductionData& data) noexcept
{
  for (auto relationship : data.relationships)
  {
    const std::shared_ptr<Plant> seller = std::get<0>(relationship);
    const std::shared_ptr<Plant> buyer = std::get<1>(relationship);
    const std::shared_ptr<Good> good = std::get<2>(relationship);
    const float sold_quantity = std::get<3>(relationship);
    const float unit_price = std::get<4>(relationship);
    grafo.insert_node(seller);
    grafo.insert_node(buyer);
    grafo.insert_arc(std::make_tuple(seller, buyer, good, sold_quantity, unit_price));
  }
}

ProductionNetwork::~ProductionNetwork()
{

}

std::vector<ProductionNetwork::NodeData> ProductionNetwork::get_node_info_list() const noexcept
{
  std::vector<ProductionNetwork::NodeData> node_list;
  std::map<std::shared_ptr<Plant>, std::shared_ptr<Plant>> m;
  
  grafo.iterate_nodes([&](DirectedGraph::Node node) {
    m[node] = node;
  });

  for(auto& _m: m){
    node_list.push_back(_m.first);
  }

  return node_list;
}

std::vector<ProductionNetwork::ArcData> ProductionNetwork::get_arc_info_list() const noexcept
{
  std::vector<ProductionNetwork::ArcData> arc_list;
  grafo.iterate_arcs([&](DirectedGraph::Arc arc) {
    arc_list.push_back(arc);
  });
  return arc_list;
}

std::vector<ProductionNetwork::ChangedPriceData> ProductionNetwork::change_price(std::shared_ptr<Good> good, float change)
{
  const auto& first_nodes_list = grafo.get_first_level_nodes();
  for (const auto& _pair : first_nodes_list) {
    auto& node = std::get<1>(_pair);
    std::cout<<"node: "<<node.get()<<"\n";
    // std::cout<<"node: "<<node.get()<<" good: "<<good.get()<<"\n";
    // if(node == good){
    //   std::cout<<"node: "<<node.get()<<" good: "<<good.get()<<"\n";
    // }
  }
  return std::vector<ChangedPriceData>{};
} 

std::ostream& ProductionNetwork::to_dot(std::ostream& output) const noexcept
{
  // TODO  
  return output;
}