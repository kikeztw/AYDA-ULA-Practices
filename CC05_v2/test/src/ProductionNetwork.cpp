#include <bits/stdc++.h>

#include <Generator.hpp>
#include <IOUtils.hpp>
#include <Models.hpp>
#include <TestUtils.hpp>

#include <ProductionNetwork.hpp>

using namespace std;

struct Data
{
  static constexpr float change{20.f};
  static shared_ptr<Good> good;
  static ProductionData pd;

  static vector<ProductionNetwork::NodeData> expected_nodes_info;
  static vector<ProductionNetwork::ArcData> expected_arcs_info;
  static vector<ProductionNetwork::ChangedPriceData> expected_changes;
};

shared_ptr<Good> Data::good{};
ProductionData Data::pd{};
vector<ProductionNetwork::NodeData> Data::expected_nodes_info{};
vector<ProductionNetwork::ArcData> Data::expected_arcs_info{};
vector<ProductionNetwork::ChangedPriceData> Data::expected_changes{};

void populate_network()
{
  ModelBuilder mb{};

  auto g1 = mb.create_good("G1");
  auto g2 = mb.create_good("G2");
  auto g3 = mb.create_good("G3");
  auto g4 = mb.create_good("G4");
  auto g5 = mb.create_good("G5");
  auto g6 = mb.create_good("G6");

  Data::good = g1;

  Data::pd.goods.insert(g1);
  Data::pd.goods.insert(g2);
  Data::pd.goods.insert(g3);
  Data::pd.goods.insert(g4);
  Data::pd.goods.insert(g5);
  Data::pd.goods.insert(g6);

  std::vector<std::tuple<std::shared_ptr<Good>, float>> inputs{};
  auto p1 = mb.create_process(g1, inputs);
  auto p2 = mb.create_process(g2, inputs);
  auto p3 = mb.create_process(g3, inputs);

  inputs.push_back(make_pair(g1, 1.238f));
  inputs.push_back(make_pair(g3, 0.266f));
  auto p4 = mb.create_process(g4, inputs);

  inputs.clear();

  inputs.push_back(make_pair(g1, 0.712f));
  inputs.push_back(make_pair(g3, 1.116f));
  auto p5 = mb.create_process(g5, inputs);

  inputs.clear();

  inputs.push_back(make_pair(g1, 0.35f));
  inputs.push_back(make_pair(g2, 1.763f));
  inputs.push_back(make_pair(g3, 0.776f));
  inputs.push_back(make_pair(g4, 0.437f));
  inputs.push_back(make_pair(g5, 1.088f));
  auto p6 = mb.create_process(g6, inputs);

  Data::pd.processes.insert(p1);
  Data::pd.processes.insert(p2);
  Data::pd.processes.insert(p3);
  Data::pd.processes.insert(p4);
  Data::pd.processes.insert(p5);
  Data::pd.processes.insert(p6);

  auto c1 = mb.create_company("J450987626");
  auto c2 = mb.create_company("G408515026");
  auto c3 = mb.create_company("V685613561");
  auto c4 = mb.create_company("J299100333");

  Data::pd.companies.insert(c1);
  Data::pd.companies.insert(c2);
  Data::pd.companies.insert(c3);
  Data::pd.companies.insert(c4);

  std::vector<std::tuple<std::shared_ptr<Process>, float, float>> production{};
  production.push_back(make_tuple(p3, 2018.548f, 95.f));
  auto pl1 = mb.create_plant(c1, production);

  production.clear();
  
  production.push_back(make_tuple(p1, 1666.844f, 87.f));
  production.push_back(make_tuple(p2, 1763.94f, 90.f));
  auto pl2 = mb.create_plant(c1, production);

  production.clear();

  production.push_back(make_tuple(p4, 437.08f, 60.f));
  auto pl3 = mb.create_plant(c2, production);

  production.clear();

  production.push_back(make_tuple(p5, 1088.58f, 98.f));
  auto pl4 = mb.create_plant(c3, production);

  production.clear();

  production.push_back(make_tuple(p6, 1000.f, 100.f));
  auto pl5 = mb.create_plant(c4, production);

  Data::pd.plants.insert(pl1);
  Data::pd.plants.insert(pl2);
  Data::pd.plants.insert(pl3);
  Data::pd.plants.insert(pl4);
  Data::pd.plants.insert(pl5);

  Data::expected_nodes_info.push_back(pl1);
  Data::expected_nodes_info.push_back(pl2);
  Data::expected_nodes_info.push_back(pl3);
  Data::expected_nodes_info.push_back(pl4);
  Data::expected_nodes_info.push_back(pl5);

  Data::pd.relationships.push_back(make_tuple(pl1, pl3, g3, 116.513f, 63.7f));
  Data::pd.relationships.push_back(make_tuple(pl1, pl4, g3, 1215.18f, 63.7f));
  Data::pd.relationships.push_back(make_tuple(pl1, pl5, g3, 776.855f, 63.7f));
  Data::pd.relationships.push_back(make_tuple(pl2, pl3, g1, 540.955f, 85.8f));
  Data::pd.relationships.push_back(make_tuple(pl2, pl4, g1, 775.171f, 85.8f));
  Data::pd.relationships.push_back(make_tuple(pl2, pl5, g1, 350.718f, 85.8f));
  Data::pd.relationships.push_back(make_tuple(pl2, pl5, g2, 1763.94f, 70.2f));
  Data::pd.relationships.push_back(make_tuple(pl3, pl5, g4, 437.08f, 160.123f));
  Data::pd.relationships.push_back(make_tuple(pl4, pl5, g5, 1088.58f, 171.867f));

  Data::expected_arcs_info.push_back(make_tuple(pl1, pl3, g3, 116.513f, 63.7f));
  Data::expected_arcs_info.push_back(make_tuple(pl1, pl4, g3, 1215.18f, 63.7f));
  Data::expected_arcs_info.push_back(make_tuple(pl1, pl5, g3, 776.855f, 63.7f));
  Data::expected_arcs_info.push_back(make_tuple(pl2, pl3, g1, 540.955f, 85.8f));
  Data::expected_arcs_info.push_back(make_tuple(pl2, pl4, g1, 775.171f, 85.8f));
  Data::expected_arcs_info.push_back(make_tuple(pl2, pl5, g1, 350.718f, 85.8f));
  Data::expected_arcs_info.push_back(make_tuple(pl2, pl5, g2, 1763.94f, 70.2f));
  Data::expected_arcs_info.push_back(make_tuple(pl3, pl5, g4, 437.08f, 160.123f));
  Data::expected_arcs_info.push_back(make_tuple(pl4, pl5, g5, 1088.58f, 171.867f));

  Data::expected_changes.push_back(make_tuple(c1, c2, g1, 85.8f, 105.8f, 23.31f));
  Data::expected_changes.push_back(make_tuple(c1, c3, g1, 85.8f, 105.8f, 23.31f));
  Data::expected_changes.push_back(make_tuple(c1, c4, g1, 85.8f, 105.8f, 23.31f));
  Data::expected_changes.push_back(make_tuple(c2, c4, g4, 160.123f, 192.313f, 20.1032f));
  Data::expected_changes.push_back(make_tuple(c3, c4, g5, 171.867f, 190.383f, 10.7733f));
}

void test()
{
  cout << "Given a network built from a production data,\n"
       << "When the method get_node_info_list is called, it should return\n"
       << Data::expected_nodes_info << endl
       << "When the method get_arc_info_list is called, it should return\n"
       << Data::expected_arcs_info << endl
       << "When the method change_price(" << Data::good << ", " << Data::change << ") is called, it should return\n"
       << Data::expected_changes << endl;

  ProductionNetwork production_network{Data::pd};

  cout << "Testing get_node_info_list\n";
  auto current_nodes_info = production_network.get_node_info_list();
  assert_equal(current_nodes_info, Data::expected_nodes_info, "get_node_info_list");

  cout << "Testing get_arc_info_list\n";
  auto current_arcs_info = production_network.get_arc_info_list();
  assert_equal(current_arcs_info, Data::expected_arcs_info, "get_arc_info_list");

  cout << "Testing change_price\n";
  auto current_changes = production_network.change_price(Data::good, Data::change);
  assert_equal(current_changes, Data::expected_changes, "change_price");

  test_report();
}

int main()
{
  cout << setprecision(2) << fixed;
  cout << "Testing Production Network\n";
  populate_network();
  test();
  return EXIT_SUCCESS;
}
