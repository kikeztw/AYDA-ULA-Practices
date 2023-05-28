#include <cassert>

#include <IOUtil.hpp>
#include <TestUtil.hpp>

#include <MuddyCity.hpp>

using namespace std;
using namespace Designar;

void set_up()
{
  BridgeBuildInfo::base_price = 500.f;
  BridgeBuildInfo::km_price = 15.f;

  TunnelBuildInfo::base_price = 200.f;
  TunnelBuildInfo::km_price = 70.f;

  RailwayBuildInfo::km_price = 10.f;
}

float distance(RailwayMap::Arc* a)
{
  return distance(a->get_src_node()->get_info(), a->get_tgt_node()->get_info());
}

struct Data
{
  static const Designar::DynArray<Town> towns;
  static const MuddyCity muddy_city;
  static const string town;
  static const string non_existing_town;
  static constexpr float min_dist{200.f};
  static const DynArray<RoadConnection> road_connections;
  static RailwayMap _unconnected_railway_map;
  static RailwayMap unconnected_railway_map();
  
  static const Point expected_town_location;
  static constexpr float expected_min_connectivity_distance{276.f};
  static CityMap _expected_city_map;
  static CityMap expected_city_map();
  static RailwayMap _expected_railway_map;
  static RailwayMap expected_railway_map();
  static RailwayMap _expected_cheapest_railroad;
  static RailwayMap expected_cheapest_railroad();
  static constexpr size_t expected_city_map_num_blocks{3};
  static constexpr size_t expected_railway_map_num_blocks{1};
  static SLList<SLList<CityMap::Node*>> expected_city_map_blocks;
  static SLList<SLList<RailwayMap::Node*>> expected_railway_map_blocks;
};

const Designar::DynArray<Town> Data::towns
{
  make_tuple("Llyne", make_tuple(100.f, 500.f)),
  make_tuple("Archmouth", make_tuple(40.f, 300.f)),
  make_tuple("Wealdstone", make_tuple(0.f, 50.f)),
  make_tuple("Lakeshore", make_tuple(150.f, 420.f)),
  make_tuple("Icemeet", make_tuple(120.f, 130.f)),
  make_tuple("Woodhaerst", make_tuple(235.f, 490.f)),
  make_tuple("Hurtlepool", make_tuple(210.f, 255.f)),
  make_tuple("Aucteraden", make_tuple(190.f, 0.f)),
  make_tuple("Auctermunty", make_tuple(480.f, 395.f)),
  make_tuple("Aynor", make_tuple(460.f, 120.f))
};

const MuddyCity Data::muddy_city{Data::towns};

const string Data::town{"Woodhaerst"};
const string Data::non_existing_town{"Bellmoral"};
const DynArray<RoadConnection> Data::road_connections
{
  make_tuple("Llyne", "Archmouth", make_tuple("Road 1", 220.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Llyne", "Woodhaerst", make_tuple("Road 2", 150.f, DynArray<Mountain>{make_tuple(25.f, 35.f), make_tuple(20.f, 60.f)}, DynArray<Gap>{})),
  make_tuple("Llyne", "Lakeshore", make_tuple("Road 3", 110.f, DynArray<Mountain>{}, DynArray<Gap>{make_tuple(80.f, Inf)})),
  make_tuple("Archmouth", "Wealdstone", make_tuple("Road 4", 260.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Archmouth", "Icemeet", make_tuple("Road 5", 200.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Archmouth", "Lakeshore", make_tuple("Road 6", 170.f, DynArray<Mountain>{}, DynArray<Gap>{make_tuple(120.f, Inf)})),
  make_tuple("Wealdstone", "Aucteraden", make_tuple("Road 7", 230.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Wealdstone", "Icemeet", make_tuple("Road 8", 145.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Woodhaerst", "Lakeshore", make_tuple("Road 9", 112.f, DynArray<Mountain>{}, DynArray<Gap>{make_tuple(100.f, Inf)})),
  make_tuple("Woodhaerst", "Hurtlepool", make_tuple("Road 10", 240.f, DynArray<Mountain>{make_tuple(30.f, 80.f), make_tuple(60.f, 30.f)}, DynArray<Gap>{})),
  make_tuple("Woodhaerst", "Aynor", make_tuple("Road 11", 450.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Woodhaerst", "Auctermunty", make_tuple("Road 12", 265.f, DynArray<Mountain>{make_tuple(180.f, 300.f)}, DynArray<Gap>{})),
  make_tuple("Hurtlepool", "Lakeshore", make_tuple("Road 13", 180.f, DynArray<Mountain>{make_tuple(20.f, 40.f)}, DynArray<Gap>{make_tuple(60.f, Inf)})),
  make_tuple("Icemeet", "Lakeshore", make_tuple("Road 13", 298.f, DynArray<Mountain>{}, DynArray<Gap>{make_tuple(150.f, Inf)})),
  make_tuple("Hurtlepool", "Icemeet", make_tuple("Road 14", 157.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Hurtlepool", "Aucteraden", make_tuple("Road 15", 264.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Hurtlepool", "Aynor", make_tuple("Road 16", 290.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Aucteraden", "Aynor", make_tuple("Road 17", 300.f, DynArray<Mountain>{}, DynArray<Gap>{})),
  make_tuple("Auctermunty", "Aynor", make_tuple("Road 18", 280.f, DynArray<Mountain>{}, DynArray<Gap>{}))
};

RailwayMap Data::_unconnected_railway_map{};

RailwayMap Data::unconnected_railway_map()
{
  if (Data::_unconnected_railway_map.get_num_nodes() == 0)
  {
    auto n0 = Data::_unconnected_railway_map.insert_node(Data::towns[0]);
    auto n1 = Data::_unconnected_railway_map.insert_node(Data::towns[1]);
    auto n2 = Data::_unconnected_railway_map.insert_node(Data::towns[2]);
    auto n3 = Data::_unconnected_railway_map.insert_node(Data::towns[3]);
    auto n4 = Data::_unconnected_railway_map.insert_node(Data::towns[4]);
    auto n5 = Data::_unconnected_railway_map.insert_node(Data::towns[5]);
    auto n6 = Data::_unconnected_railway_map.insert_node(Data::towns[6]);
    auto n7 = Data::_unconnected_railway_map.insert_node(Data::towns[7]);
    auto n8 = Data::_unconnected_railway_map.insert_node(Data::towns[8]);
    auto n9 = Data::_unconnected_railway_map.insert_node(Data::towns[9]);

    Data::_unconnected_railway_map.insert_arc(n0, n1, make_tuple("Road 1", 220.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_unconnected_railway_map.insert_arc(n0, n5, make_tuple("Road 2", 150.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_unconnected_railway_map.insert_arc(n0, n3, make_tuple("Road 3", 110.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_unconnected_railway_map.insert_arc(n1, n4, make_tuple("Road 5", 200.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_unconnected_railway_map.insert_arc(n3, n4, make_tuple("Road 5", 200.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_unconnected_railway_map.insert_arc(n2, n7, make_tuple("Road 7", 230.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_unconnected_railway_map.insert_arc(n5, n6, make_tuple("Road 10", 240.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_unconnected_railway_map.insert_arc(n5, n9, make_tuple("Road 11", 450.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_unconnected_railway_map.insert_arc(n5, n8, make_tuple("Road 12", 265.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_unconnected_railway_map.insert_arc(n8, n9, make_tuple("Road 12", 265.f, DynArray<Mountain>{}, DynArray<Gap>{}));
  }

  return Data::_unconnected_railway_map;
}

const Point Data::expected_town_location{235.f, 490.f};
CityMap Data::_expected_city_map{};

CityMap Data::expected_city_map()
{
  if (Data::_expected_city_map.get_num_nodes() == 0)
  {
    auto n0 = Data::_expected_city_map.insert_node(Data::towns[0]);
    auto n1 = Data::_expected_city_map.insert_node(Data::towns[1]);
    auto n2 = Data::_expected_city_map.insert_node(Data::towns[2]);
    auto n3 = Data::_expected_city_map.insert_node(Data::towns[3]);
    auto n4 = Data::_expected_city_map.insert_node(Data::towns[4]);
    auto n5 = Data::_expected_city_map.insert_node(Data::towns[5]);
    auto n6 = Data::_expected_city_map.insert_node(Data::towns[6]);
    auto n7 = Data::_expected_city_map.insert_node(Data::towns[7]);
    auto n8 = Data::_expected_city_map.insert_node(Data::towns[8]);
    auto n9 = Data::_expected_city_map.insert_node(Data::towns[9]);

    Data::_expected_city_map.insert_arc(n1, n3);
    Data::_expected_city_map.insert_arc(n7, n4);
    Data::_expected_city_map.insert_arc(n7, n2);
    Data::_expected_city_map.insert_arc(n6, n1);
    Data::_expected_city_map.insert_arc(n6, n3);
    Data::_expected_city_map.insert_arc(n4, n1);
    Data::_expected_city_map.insert_arc(n4, n6);
    Data::_expected_city_map.insert_arc(n3, n0);
    Data::_expected_city_map.insert_arc(n3, n5);
    Data::_expected_city_map.insert_arc(n2, n4);
    Data::_expected_city_map.insert_arc(n5, n0);

    Data::expected_city_map_blocks.append(SLList<CityMap::Node*>{n1, n7, n6, n4, n3, n0, n2, n5});
    Data::expected_city_map_blocks.append(SLList<CityMap::Node*>{n8});
    Data::expected_city_map_blocks.append(SLList<CityMap::Node*>{n9});
  }

  return Data::_expected_city_map;
}

RailwayMap Data::_expected_railway_map{};

RailwayMap Data::expected_railway_map()
{
  if (Data::_expected_railway_map.get_num_nodes() == 0)
  {
    auto n0 = Data::_expected_railway_map.insert_node(Data::towns[0]);
    auto n1 = Data::_expected_railway_map.insert_node(Data::towns[1]);
    auto n2 = Data::_expected_railway_map.insert_node(Data::towns[2]);
    auto n3 = Data::_expected_railway_map.insert_node(Data::towns[3]);
    auto n4 = Data::_expected_railway_map.insert_node(Data::towns[4]);
    auto n5 = Data::_expected_railway_map.insert_node(Data::towns[5]);
    auto n6 = Data::_expected_railway_map.insert_node(Data::towns[6]);
    auto n7 = Data::_expected_railway_map.insert_node(Data::towns[7]);
    auto n8 = Data::_expected_railway_map.insert_node(Data::towns[8]);
    auto n9 = Data::_expected_railway_map.insert_node(Data::towns[9]);

    Data::_expected_railway_map.insert_arc(n0, n1, make_tuple("Road 1", 220.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n0, n5, make_tuple("Road 2", 150.f, DynArray<Mountain>{make_tuple(25.f, 35.f), make_tuple(20.f, 60.f)}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n0, n3, make_tuple("Road 3", 110.f, DynArray<Mountain>{}, DynArray<Gap>{make_tuple(80.f, Inf)}));
    Data::_expected_railway_map.insert_arc(n1, n2, make_tuple("Road 4", 260.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n1, n4, make_tuple("Road 5", 200.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n1, n3, make_tuple("Road 6", 170.f, DynArray<Mountain>{}, DynArray<Gap>{make_tuple(120.f, Inf)}));
    Data::_expected_railway_map.insert_arc(n2, n7, make_tuple("Road 7", 230.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n2, n4, make_tuple("Road 8", 145.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n5, n3, make_tuple("Road 9", 112.f, DynArray<Mountain>{}, DynArray<Gap>{make_tuple(100.f, Inf)}));
    Data::_expected_railway_map.insert_arc(n5, n6, make_tuple("Road 10", 240.f, DynArray<Mountain>{make_tuple(30.f, 80.f), make_tuple(60.f, 30.f)}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n5, n9, make_tuple("Road 11", 450.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n5, n8, make_tuple("Road 12", 265.f, DynArray<Mountain>{make_tuple(180.f, 300.f)}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n6, n3, make_tuple("Road 13", 180.f, DynArray<Mountain>{make_tuple(20.f, 40.f)}, DynArray<Gap>{make_tuple(60.f, Inf)}));
    Data::_expected_railway_map.insert_arc(n4, n3, make_tuple("Road 13", 298.f, DynArray<Mountain>{}, DynArray<Gap>{make_tuple(150.f, Inf)}));
    Data::_expected_railway_map.insert_arc(n6, n4, make_tuple("Road 14", 157.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n6, n7, make_tuple("Road 15", 264.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n6, n9, make_tuple("Road 16", 290.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n7, n9, make_tuple("Road 17", 300.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_railway_map.insert_arc(n8, n9, make_tuple("Road 18", 280.f, DynArray<Mountain>{}, DynArray<Gap>{}));

    Data::expected_railway_map_blocks.append(SLList<RailwayMap::Node*>{n1, n7, n8, n9, n6, n4, n3, n0, n2, n5});
  }

  return Data::_expected_railway_map;
}

RailwayMap Data::_expected_cheapest_railroad{};

RailwayMap Data::expected_cheapest_railroad()
{
  if (Data::_expected_cheapest_railroad.get_num_nodes() == 0)
  {
    auto n0 = Data::_expected_cheapest_railroad.insert_node(Data::towns[0]);
    auto n1 = Data::_expected_cheapest_railroad.insert_node(Data::towns[1]);
    auto n2 = Data::_expected_cheapest_railroad.insert_node(Data::towns[2]);
    auto n3 = Data::_expected_cheapest_railroad.insert_node(Data::towns[3]);
    auto n4 = Data::_expected_cheapest_railroad.insert_node(Data::towns[4]);
    auto n5 = Data::_expected_cheapest_railroad.insert_node(Data::towns[5]);
    auto n6 = Data::_expected_cheapest_railroad.insert_node(Data::towns[6]);
    auto n7 = Data::_expected_cheapest_railroad.insert_node(Data::towns[7]);
    auto n8 = Data::_expected_cheapest_railroad.insert_node(Data::towns[8]);
    auto n9 = Data::_expected_cheapest_railroad.insert_node(Data::towns[9]);

    Data::_expected_cheapest_railroad.insert_arc(n0, n1, make_tuple("Road 1", 220.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_cheapest_railroad.insert_arc(n0, n5, make_tuple("Road 2", 150.f, DynArray<Mountain>{make_tuple(25.f, 35.f), make_tuple(20.f, 60.f)}, DynArray<Gap>{}));
    Data::_expected_cheapest_railroad.insert_arc(n0, n3, make_tuple("Road 3", 110.f, DynArray<Mountain>{}, DynArray<Gap>{make_tuple(80.f, Inf)})); 
    Data::_expected_cheapest_railroad.insert_arc(n1, n4, make_tuple("Road 5", 200.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_cheapest_railroad.insert_arc(n2, n7, make_tuple("Road 7", 230.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_cheapest_railroad.insert_arc(n2, n4, make_tuple("Road 8", 145.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_cheapest_railroad.insert_arc(n6, n4, make_tuple("Road 14", 157.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_cheapest_railroad.insert_arc(n6, n9, make_tuple("Road 16", 290.f, DynArray<Mountain>{}, DynArray<Gap>{}));
    Data::_expected_cheapest_railroad.insert_arc(n8, n9, make_tuple("Road 18", 280.f, DynArray<Mountain>{}, DynArray<Gap>{}));
  }

  return Data::_expected_cheapest_railroad;
}

SLList<SLList<CityMap::Node*>> Data::expected_city_map_blocks{};

SLList<SLList<RailwayMap::Node*>> Data::expected_railway_map_blocks{};

void test_town_location()
{
  cout << "Given the town " << Data::town << ", when the method get_town_location is called, "
       << "it should return " << Data::expected_town_location << endl;

  auto current = Data::muddy_city.get_town_location(Data::town);
  assert_equal(current, Data::expected_town_location);
}

void test_town_location_throw_exception()
{
  cout << "Given the town " << Data::non_existing_town << ", when the method get_town_location is called, "
       << "the exception TownNotFoundException should be thrown." << endl;

  auto call_get_town_location = [] (const std::string& town_name) { return Data::muddy_city.get_town_location(town_name); };

  assert_throw_exception<TownNotFoundException>(call_get_town_location, Data::non_existing_town);
}

void test_min_connectivity_distance()
{
  cout << "When the method min_connectivity_distance is called, it should return "
       << Data::expected_min_connectivity_distance << endl;

  auto current = Data::muddy_city.min_connectivity_distance();
  assert_equal(current, Data::expected_min_connectivity_distance);
}

void test_build_city_map()
{
  cout << "Given the min_distance " << Data::min_dist << ", when the method build_city_map is called, "
       << "it should return the graph" << endl
       << Data::expected_city_map() << endl;

  auto current = Data::muddy_city.build_city_map(Data::min_dist);
  assert_equal(current, Data::expected_city_map());
}

void test_build_railway_map()
{
  cout << "Given the road connections:\n"
       << Data::road_connections << endl
       << "When the method build_railway_map is called, it should return the graph" << endl
       << Data::expected_railway_map() << endl;

  auto current = Data::muddy_city.build_railway_map(Data::road_connections);
  assert_equal(current, Data::expected_railway_map());
}

void test_cheapest_railroad()
{
  cout << "Given the railway map:\n"
       << Data::expected_railway_map() << endl
       << "When the method cheapest_railroad is called, it should return the tree" << endl
       << Data::expected_cheapest_railroad() << endl;

  auto current = Data::muddy_city.cheapest_railroad(Data::expected_railway_map());
  assert_equal(current, Data::expected_cheapest_railroad());
}

void test_cheapest_railroad_throw_exception()
{
  cout << "Given the railway map:\n"
       << Data::unconnected_railway_map() << endl
       << "When the method cheapest_railroad is called, the exception UnconnectedMapException should be thrown" << endl;

  auto call_cheapest_railroad = [] (const RailwayMap& railway_map) { return Data::muddy_city.cheapest_railroad(railway_map); };
  assert_throw_exception<UnconnectedMapException>(call_cheapest_railroad, Data::unconnected_railway_map());
}

void test_city_map_num_blocks()
{
  cout << "Given the city map:\n"
       << Data::expected_city_map() << endl
       << "When the method num_blocks is called, it should return " << Data::expected_city_map_num_blocks << endl;

  auto current = Data::muddy_city.num_blocks(Data::expected_city_map());
  assert_equal(current, Data::expected_city_map_num_blocks);
}

void test_railway_map_num_blocks()
{
  cout << "Given the railway map:\n"
       << Data::expected_railway_map() << endl
       << "When the method num_blocks is called, it should return " << Data::expected_railway_map_num_blocks << endl;

  auto current = Data::muddy_city.num_blocks(Data::expected_railway_map());
  assert_equal(current, Data::expected_railway_map_num_blocks);
}

void test_city_map_blocks()
{
  cout << "Given the city map:\n"
       << Data::expected_city_map() << endl
       << "When the method blocks is called, it should return\n"
       << Data::expected_city_map_blocks << endl;

  auto current = Data::muddy_city.blocks(Data::expected_city_map());
  assert_equal(current, Data::expected_city_map_blocks);
}

void test_railway_blocks()
{
  cout << "Given the railway map:\n"
       << Data::expected_railway_map() << endl
       << "When the method blocks is called, it should return\n"
       << Data::expected_railway_map_blocks << endl;

  auto current = Data::muddy_city.blocks(Data::expected_railway_map());
  assert_equal(current, Data::expected_railway_map_blocks);
}

int main()
{
  set_up();
  test_town_location();
  test_town_location_throw_exception();
  test_min_connectivity_distance();
  test_build_city_map();
  test_build_railway_map();
  test_cheapest_railroad();
  test_cheapest_railroad_throw_exception();
  test_city_map_num_blocks();
  test_railway_map_num_blocks();
  test_city_map_blocks();
  test_railway_blocks();

  cout << "Everything ok!\n";

  return EXIT_SUCCESS;
}
