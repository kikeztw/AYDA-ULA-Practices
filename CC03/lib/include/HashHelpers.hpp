/**
 * Hash combine implementation from https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3876.pdf
 */
#pragma once

#include <functional>

template <typename T> 
void hash_combine(size_t& seed, const T& val)
{
  seed ^= std::hash<T>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T, typename ...Ts>
void hash_combine(size_t& seed, const T& val, const Ts&... args)
{
  hash_combine(seed, val);
  hash_combine(seed, args...);
}

void hash_combine(size_t& seed);

template <typename ...Ts>
size_t hash_val(const Ts&... args)
{
  size_t seed{0};
  hash_combine(seed, args...);
  return seed;
}
