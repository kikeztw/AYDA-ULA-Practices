/**
 * @file Rng.hpp
 * @author Alejandro Mujica (aledrums@gmail.com)
 * @copyright Copyright (c) 2022
 */
#pragma once

#include <random>

/// Singleton for random number generation.
class Rng
{
public:
  /// Generates a random real number in [0, 1).
  static float random_uniform() noexcept;

  /**
   * @brief  Generates a random integer number in [l, r].
   * 
   * @tparam T: The integral type.
   * @param l: The lowerbound. 
   * @param r: The upperbound.
   * @return the generated random number. 
   */
  template <typename T>
  static T random_uniform_int(T l, T r) noexcept;

  /**
   * @brief  Generates a random real number in [l, r].
   * 
   * @tparam T: The floating-point type.
   * @param l: The lowerbound. 
   * @param r: The upperbound.
   * @return the generated random number. 
   */
  template <typename T>
  static T random_uniform_real(T l, T r) noexcept;

  /**
   * @brief Generates a random number normally distributed.
   * 
   * @tparam T: The floating-point type.
   * @param mu: Mean.
   * @param sigma: Standard deviation.
   * @return the generated random number.
   */
  template <typename T>
  static T random_normal(T mu, T sigma) noexcept;

  /**
   * @brief Generates a random number exponentially distributed.
   * 
   * @tparam T: The floating-point type.
   * @param mu: Mean.
   * @return the generated random number.
   */
  template <typename T>
  static T random_exponential(T mu) noexcept;

  /**
   * @brief Simulates a coin toss.
   * 
   * This uses the Bernoulli distribution to simulate a coin toss.
   * 
   * @param p: The probability of getting true as the result. By default is 0.5 (fair coin).
   * @return true for p.
   * @return false for 1 - p.
   */
  static bool flip_coin(float p = 0.5) noexcept;

  /**
   * @brief Selects an item randomly between two iterators.
   * 
   * @tparam It: The iterator type.
   * @param begin: The iterator on the start position.
   * @param end: The iterator on the final position.
   * @return The iterator on the selected item.
   */
  template <typename It>
  static It random_choice(It begin, It end) noexcept;

  /**
   * @brief Generates a random string with the R.I.F.-format.
   * 
   * @return The generates R.I.F.
   */
  static std::string random_rif() noexcept;

  static void set_seed(std::mt19937::result_type s) noexcept;

private:
  static std::random_device rd;
  static std::mt19937 rng;
};

template <typename T>
T Rng::random_uniform_int(T l, T r) noexcept
{
  static_assert(std::is_integral<T>::value, "Template argument should be an integral type");
  std::uniform_int_distribution<T> dist{l, r};
  return dist(Rng::rng);
}

template <typename T>
T Rng::random_uniform_real(T l, T r) noexcept
{
  static_assert(std::is_floating_point<T>::value, "Template argument should be a floating point type");
  std::uniform_real_distribution<T> dist{l, r};
  return dist(Rng::rng);
}

template <typename T>
T Rng::random_normal(T mu, T sigma) noexcept
{
  static_assert(std::is_floating_point<T>::value, "Template argument should be a floating point type");
  std::normal_distribution<T> dist{mu, sigma};
  return dist(Rng::rng);
}

template <typename T>
T Rng::random_exponential(T mu) noexcept
{
  static_assert(std::is_floating_point<T>::value, "Template argument should be a floating point type");
  std::exponential_distribution<T> dist{mu};
  return 1.f/dist(Rng::rng);
}

template <typename It>
It Rng::random_choice(It begin, It end) noexcept
{
  auto size = std::distance(begin, end);
  auto i = random_uniform_int<decltype(size)>(0, size - 1);

  while (i-- > 0)
  {
    ++begin;
  }

  return begin;
}
