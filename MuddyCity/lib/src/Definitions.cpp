#include <Definitions.hpp>

float TunnelBuildInfo::base_price{0.f};
float TunnelBuildInfo::km_price{0.f};

float BridgeBuildInfo::base_price{0.f};
float BridgeBuildInfo::km_price{0.f};

float RailwayBuildInfo::km_price{0.f};

float distance(const Point& p1, const Point& p2)
{
  float x1 = std::get<0>(p1);
  float x2 = std::get<0>(p2);
  float y1 = std::get<1>(p1);
  float y2 = std::get<1>(p2);
  float xlen = x1 - x2;
  float ylen = y1 - y2;

  return std::hypot(xlen, ylen);
}

float distance(const Town& t1, const Town& t2)
{
  return distance(std::get<1>(t1), std::get<1>(t2));
}