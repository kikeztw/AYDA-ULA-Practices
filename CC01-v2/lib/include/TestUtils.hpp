#pragma once

#include <algorithm>

#include <IOUtils.hpp>

class Color;

bool operator == (const Color& c1, const Color& c2);

bool operator != (const Color& c1, const Color& c2);

template <class Vector, class Equal = std::equal_to<typename Vector::value_type>>
bool equal_vector(const Vector& v1, const Vector& v2, Equal eq = Equal{})
{
  if (v1.size() != v2.size())
  {
    return false;
  }

  return std::equal(v1.begin(), v1.end(), v2.begin(), eq);
}

template <typename T>
void assert_equal(const T& current, const T& expected)
{
  if (current != expected)
  {
    std::cerr << "Test failed:\n";
    std::cerr << "Current: " << current << std::endl;
    std::cerr << "Expected: " << expected << std::endl;
    abort();
  }
}

template <class Vector>
void assert_equal_vector(const Vector& current, const Vector& expected)
{
  if (!equal_vector(expected, current))
  {
    std::cerr << "Test failed:\n";
    std::cerr << "Current: ";
    print_vector(current, std::cerr);
    std::cerr << "Expected: ";
    print_vector(expected, std::cerr);
    abort();
  }
}

template <class Matrix>
void assert_equal_matrix(const Matrix& current, const Matrix& expected)
{
  if (!equal_vector(expected, current, [] (const auto& v1, const auto& v2) { return equal_vector(v1, v2); }))
  {
    std::cerr << "Test failed:\n";
    std::cerr << "Current: ";
    print_matrix(current, std::cerr);
    std::cerr << "Expected: ";
    print_matrix(expected, std::cerr);
    abort();
  }
}

