#pragma once

#include <string>
#include <tuple>

#include <array.hpp>
#include <graph.hpp>

static constexpr float Inf = std::numeric_limits<float>::max();

/** Alias to represent an euclidean point.
 * 
 * Components:
 * - The first element is the X component.
 * - The second element is the Y component.
 * 
 * Usage example:
 * \code{.cpp}
 * Point location = std::make_tuple(10.f, 20.f);
 * float x = std::get<0>(location); // x is 10.f
 * float y = std::get<1>(location); // x is 20.f
 * \endcode
 * 
 * \see City
 */
using Point = std::tuple<float, float>;

/** Alias to represent a town.
 * 
 * Components:
 * - The first element is the unique name of the city.
 * - The second element is the town location.
 * 
 * Usage example:
 * \code{.cpp}
 * Town town = std::make_tuple("Town 1", std::make_tuple(10.f, 20.f));
 * 
 * // Get the name:
 * std::string name = std::get<0>(town); // name is "Town 1"
 * 
 * // Get the location components:
 * float x = std::get<0>(std::get<1>(town)); // x is 10.f
 * float y = std::get<1>(std::get<1>(town)); // x is 20.f
 * 
 * // Get the location:
 * Location location = std::get<1>(town);
 * \endcode
 * 
 * \see Point
 */
using Town = std::tuple<std::string, Point>;

/** Alias to represent an road obstacle.
 * 
 * An obstacle could be a mountain or a gap.
 * 
 * You can get to the other side of a mountain by surrounding it
 * or by building a tunnel through it. You also can get to the
 * other side of a gap by surrounding it or by building a bridge.
 * The length of the tunnel or the bridge is the length of the obstacle.
 * 
 * Components:
 * - The first element is the lengh of the obstacle.
 * - The second element is the distance of surrounding the obstacle.
 * 
 * Usage example:
 * \code{.cpp}
 * Mountain mountain = std::make_tuple(100.f, 300.f);
 * float mountain_length = std::get<0>(mountain);
 * float mountain_surrounding_length = std::get<1>(mountain);
 * 
 * Gap gap = std::make_tuple(100.f, 300.f);
 * float gap_length = std::get<0>(gap);
 * float gap_surrounding_length = std::get<1>(gap);
 * \endcode 
 */ 
using Obstacle = std::tuple<float, float>;
using Mountain = Obstacle;
using Gap = Obstacle;

/** Alias to represent a road.
 * 
 * A road is a candidate to build a railway line.
 * 
 * Components:
 * - The first element is the unique name of the road.
 * - The second element is the length of the road.
 * - The third element is a list of mountains on the road.
 * - The fourth element is a list of gaps on the road.
 * 
 * Usage example:
 * \code{.cpp}
 * Road road;
 * // road initialization.
 * std::string name = std::get<0>(road);
 * float length = std::get<1>(road);
 * const std::vector<Mountain>& mountains = std::get<2>(road);
 * const std::vector<Gap>& gaps = std::get<3>(road);
 * \endcode
 * 
 * \see Obstacle, Mountain, Gap
 */
using Road = std::tuple<std::string, float, Designar::DynArray<Mountain>, Designar::DynArray<Gap>>;

/** Price info to build a tunnel.
 * 
 * The total price of building a tunnel is given by
 * 
 * \f$base_price + km_price * mountain_length\f$.
 * 
 * \see Obstacle, Mountain
 */ 
struct TunnelBuildInfo
{
  static float base_price;
  static float km_price;
};

/** Price info to build a bridge.
 * 
 * The total price of building a bridge is given by
 * 
 * \f$base_price + km_price * gap_length\f$.
 * 
 * \see Obstacle, Gap
 */ 
struct BridgeBuildInfo
{
  static float base_price;
  static float km_price;
};

/// Price info to build a railway line.
struct RailwayBuildInfo
{
  static float km_price;
};

/// Alias for a city map
using CityMap = Designar::Graph<Town>;

/// Alias for a railway map
using RailwayMap = Designar::Graph<Town, Road>;

/// Template alias for a map block.
template <class Map>
using Block = Designar::SLList<typename Map::Node*>;

/** Alias for a road connection.
 * 
 * Components:
 * - The first and the second elements are the names of the towns that are
 *   connected by the road.
 * - The third element is the info of the road.
 * 
 * \see Road
 */ 
using RoadConnection = std::tuple<std::string, std::string, Road>;


float distance(const Point& p1, const Point& p2);

float distance(const Town& t1, const Town& t2);
