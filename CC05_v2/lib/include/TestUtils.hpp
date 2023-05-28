#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include <IOUtils.hpp>

using ReportList = std::vector<std::tuple<
                                std::string, // test name
                                bool, // passed
                                std::string // Results
                              >>;

struct TestResults
{
  static ReportList results;
};

std::string result_value(bool passed);

void test_report();

#define EPSILON 10E6

bool operator < (const std::tuple<std::shared_ptr<Plant>, std::shared_ptr<Plant>, std::shared_ptr<Good>, float, float>& t1,
                 const std::tuple<std::shared_ptr<Plant>, std::shared_ptr<Plant>, std::shared_ptr<Good>, float, float>& t2);

bool operator == (const std::tuple<std::shared_ptr<Plant>, std::shared_ptr<Plant>, std::shared_ptr<Good>, float, float>& t1,
                  const std::tuple<std::shared_ptr<Plant>, std::shared_ptr<Plant>, std::shared_ptr<Good>, float, float>& t2);

bool operator < (const std::tuple<std::shared_ptr<Company>, std::shared_ptr<Company>, std::shared_ptr<Good>, float, float, float>& t1,
                 const std::tuple<std::shared_ptr<Company>, std::shared_ptr<Company>, std::shared_ptr<Good>, float, float, float>& t2);

bool operator == (const std::tuple<std::shared_ptr<Company>, std::shared_ptr<Company>, std::shared_ptr<Good>, float, float, float>& t1,
                  const std::tuple<std::shared_ptr<Company>, std::shared_ptr<Company>, std::shared_ptr<Good>, float, float, float>& t2);

template <typename T>
bool equal_value(const T& v1, const T& v2);

bool equal_value(float a, float b);

template <class Container, class Less = std::less<typename Container::value_type>>
bool equal_container(const Container& c1, const Container& c2, Less cmp = Less{})
{
  if (c1.size() != c2.size())
  {
    return false;
  }

  std::vector<typename Container::value_type> v1{c1.begin(), c1.end()};
  std::vector<typename Container::value_type> v2{c2.begin(), c2.end()};
  
  std::sort(v1.begin(), v1.end(), cmp);
  std::sort(v2.begin(), v2.end(), cmp);

  return std::equal(v1.begin(), v1.end(), v2.begin(), equal_value<typename Container::value_type>);
}

template <typename T>
bool equal_value(const T& v1, const T& v2)
{
  return v1 == v2;
}

template <typename T>
bool equal_value(const std::vector<T>& v1, const std::vector<T>& v2)
{
  return equal_container(v1, v2);
}

template <typename T>
void assert_equal(const T& current, const T& expected, const std::string& test_name)
{
  if (equal_value(expected, current))
  {
    TestResults::results.push_back(std::make_tuple(test_name, true, ""));
  }
  else
  {
    std::stringstream results;
    results << "Current result: " << current << '\n';
    results << "Expected result: " << expected << '\n';
    TestResults::results.push_back(std::make_tuple(test_name, false, results.str()));
  }
}
