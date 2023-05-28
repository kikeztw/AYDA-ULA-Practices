/**
 * @file ProductionNetwork.hpp
 * @authors 
 */

#pragma once
#include <Models.hpp>
#include <memory>
#include <tuple>
#include <vector>
#include <functional>
#include <iostream>

class DirectedGraph {
public:
    using Node = std::shared_ptr<Plant>;
    using Arc = std::tuple<Node, Node, std::shared_ptr<Good>, float, float>;

    void iterate_nodes(std::function<void(Node)> operation) const;
    void iterate_arcs(std::function<void(Arc)> operation) const;
    void insert_node(Node node);
    void insert_arc(Arc arc);
    Node get_root_node() const;
    std::vector<std::pair<DirectedGraph::Node, std::shared_ptr<Good>>> get_first_level_nodes() const;

private:
    std::vector<Node> nodes;
    std::vector<Arc> arcs;
};


class ProductionNetwork
{
public:
  /// Alias to the data type that should be stored in each node.
  using NodeInfoType = std::shared_ptr<Plant>;

  /// Alias to the data type that should be stored in each arc.
  using ArcInfoType = std::tuple<std::shared_ptr<Plant>, std::shared_ptr<Plant>, std::shared_ptr<Good>, float, float>;

  /// Alias for the type of Graph. (optional usage).
  using NetworkType = void;

  /// Aliases to retrieve data from the graph.
  using NodeData = std::shared_ptr<Plant>;
  using ArcData = std::tuple<std::shared_ptr<Plant>, std::shared_ptr<Plant>, std::shared_ptr<Good>, float, float>;

  /** Alias for changed price data
   * 
   * Components:
   * The first element is the seller company
   * The second element is the buyer company
   * The third element is the sold good.
   * The fourth element is the old price.
   * The fifth element is the new price.
   * The sixth element is the percentage of change of the price.
   */
  using ChangedPriceData = std::tuple<std::shared_ptr<Company>, std::shared_ptr<Company>, std::shared_ptr<Good>, float, float, float>;

  ProductionNetwork(const ProductionData& data) noexcept;

  ~ProductionNetwork();
  
  ProductionNetwork(const ProductionNetwork&) = delete;
  
  ProductionNetwork(const ProductionNetwork&&) = delete;
  
  ProductionNetwork& operator = (ProductionNetwork) = delete;
  
  /// Returns the list of the plants stored in nodes.
  std::vector<NodeData> get_node_info_list() const noexcept;
  
  /// Returns the list of tuples (seller, buyer, good, quantity, price) represented by the arcs
  std::vector<ArcData> get_arc_info_list() const noexcept;
  
  /**
   * @brief changes the sale price of a good.
   * 
   * Changes the sales price of a good and computes the effect of this change, this means that all of the companies that use
   * the good as input should change their products' sales prices. Then, there will be more companies affected by the change
   * in the prices of their inputs.
   *
   * @param good: The good whose price is changed.
   * @param change: The amount of price change. This could be positive (increment), negative (decrement), or zero (no change).
   * @return a list with the relationship data affected by the change. Each element in the list is a tuple as follows:
   *        (seller company, buyer company, good, old_price, new_price, percentage_of_change).
   * @throw std::invalid_argument with the message "The good should be not null" if good is nullptr.
   * @throw std::domain_error with the message "Good does not exist"  if the good is not in the production network.
   * @throw std::logic_error with the message "The good is not a first-level-good" if the good is not in level 0.
   */
  std::vector<ChangedPriceData> change_price(std::shared_ptr<Good> good, float change);
  
  /**
   * Writes on the stream the DOT representation of the production network.
   * See the documentation of Graphviz here: https://graphviz.org/documentation/
   */
  std::ostream& to_dot(std::ostream& output) const noexcept;

private:
  // Your attributes here.
  DirectedGraph grafo;
};
