#include <MuddyCity.hpp>
#include <array.hpp>
#include <iostream>
#include <Definitions.hpp>
#include <graph.hpp>
#include <vector>
#include <set>

MuddyCity::MuddyCity(const Designar::DynArray<Town>& towns) noexcept
{
  // TODO
  for (Designar::DynArray<Town>::Iterator it = towns.begin(); it != towns.end(); ++it)
  {
    towns_list.insert(*it);
  }
}

Point MuddyCity::get_town_location(const std::string& town_name) const
{
  float location_x = 0;
  float location_y = 0;
  for (Designar::DynArray<Town>::Iterator it = towns_list.begin(); it != towns_list.end(); ++it)
  {
    const std::string name = std::get<0>(*it);
    const auto current_location = std::get<1>(*it);
    if(name == town_name){
      location_x = std::get<0>(current_location);
      location_y = std::get<1>(current_location);
    }
  }

  if(location_x == 0 && location_y ==0){
    throw TownNotFoundException{town_name};
  }

  return std::make_tuple(location_x, location_y);
  // throw TownNotFoundException{town_name};
}

float MuddyCity::min_connectivity_distance() const noexcept
{
  
  return Inf;
}

CityMap MuddyCity::build_city_map(float span_dist) const noexcept
{
  Designar::Graph<Town> cityMap;
  
  for (Designar::DynArray<Town>::Iterator it = towns_list.begin(); it != towns_list.end(); ++it)
  {
    cityMap.insert_node(*it);
  }

  for (auto i = towns_list.begin(); i != towns_list.end(); ++i)
  {
    for (auto j = i + 1; j != towns_list.end(); ++j)
    {
      if (distance(*i, *j) <= span_dist) {
        auto first_town = *i;
        auto second_town = *j;
        auto first_town_name = std::get<0>(first_town);
        auto second_town_name = std::get<0>(second_town);
        auto first_node = cityMap.search_node([&](auto node){ 
          auto current_node = node->get_info();
          auto city_node_name = std::get<0>(current_node);
          return city_node_name == first_town_name
        });
        auto second_node = cityMap.search_node([&](auto node){ 
          auto current_node = node->get_info();
          auto city_node_name = std::get<0>(current_node);
          return city_node_name == second_town_name; 
        });
        cityMap.insert_arc(first_node, second_node);
      }
    }
  }
  return cityMap;
}

RailwayMap MuddyCity::build_railway_map(const Designar::DynArray<RoadConnection>& roads) const noexcept
{
  Designar::Graph<Town, Road> build_railway_map;
  
  for (Designar::DynArray<Town>::Iterator it = towns_list.begin(); it != towns_list.end(); ++it)
  {
    build_railway_map.insert_node(*it);
  }

  for (Designar::DynArray<RoadConnection>::Iterator it = roads.begin(); it != roads.end(); ++it)
  {
    auto first_town_name = std::get<0>(*it);
    auto second_town_name = std::get<1>(*it);
    auto first_node = build_railway_map.search_node([first_town_name, second_town_name](auto node){ 
      auto current_node = node->get_info();
      auto city_node_name = std::get<0>(current_node);
      return city_node_name == first_town_name
    });
    auto second_node = build_railway_map.search_node([first_town_name, second_town_name](auto node){ 
      auto current_node = node->get_info();
      auto city_node_name = std::get<0>(current_node);
      return city_node_name == second_town_name; 
    });

    build_railway_map.insert_arc(first_node, second_node);
  }

  return build_railway_map;
}

RailwayMap MuddyCity::cheapest_railroad(const RailwayMap& railway_map) const
{
  // TODO
  // throw UnconnectedMapException{};
}


//  std::string current_name = std::get<0>(*i);
//     Point current_location = std::get<1>(*i);
