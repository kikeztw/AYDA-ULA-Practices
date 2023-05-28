/**
 * \file MuddyCity.hpp
 * \authors 
 */

#pragma once
#include <set>
#include <Definitions.hpp>
#include <array.hpp>
#include <graphalgorithms.hpp>

class MuddyCity
{
public:
  MuddyCity(const Designar::DynArray<Town>& towns) noexcept;

  Point get_town_location(const std::string& town_name) const;

  float min_connectivity_distance() const noexcept;

  CityMap build_city_map(float span_dist) const noexcept;

  RailwayMap build_railway_map(const Designar::DynArray<RoadConnection>& roads) const noexcept;

  RailwayMap cheapest_railroad(const RailwayMap& railway_map) const;

  template <class Map>
  size_t num_blocks(const Map& map) const noexcept;

  template <class Map>
  size_t num_blocks(Map&& map) const noexcept;

  template <class Map>
  Designar::SLList<Block<Map>> blocks(const Map& map) const noexcept;

  template <class Map>
  Designar::SLList<Block<Map>> blocks(Map&& map) const noexcept;

private:
  Designar::DynArray<Town> towns_list;
};

template<class Map>
size_t MuddyCity::num_blocks(const Map& map) const noexcept
{
  auto block = compute_connected_components(map);
  return block.size();  
}

template<class Map>
size_t MuddyCity::num_blocks(Map&& map) const noexcept
{
   
}

template<class Map>
Designar::SLList<Block<Map>> MuddyCity::blocks(const Map& map) const noexcept
{
  // TODO
  return Designar::SLList<Block<Map>>{};
}

template<class Map>
Designar::SLList<Block<Map>> MuddyCity::blocks(Map&& map) const noexcept
{
  // Do not modify this method.
  return blocks<Map>(map);
}
