/**
 * @file Rng.cpp
 * @author Alejandro Mujica (aledrums@gmail.com)
 * @copyright Copyright (c) 2022
 */
#include <Rng.hpp>

std::random_device Rng::rd;
std::mt19937 Rng::rng{rd()};

float Rng::random_uniform() noexcept
{
  return std::generate_canonical<float, 32>(Rng::rng);
}

bool Rng::flip_coin(float p) noexcept
{
  std::bernoulli_distribution dist{p};
  return dist(Rng::rng);
}

std::string Rng::random_rif() noexcept
{
  constexpr short NUM_LETTERS = 3;
  char letters[] = { 'V', 'J', 'G' };

  std::string rif;
  rif.push_back(letters[random_uniform_int(0, NUM_LETTERS - 1)]);
  rif.push_back(random_uniform_int('1', '9'));

  for (unsigned short i = 0; i < 8; ++i)
  {
    rif.push_back(random_uniform_int('0', '9'));
  }

  return rif;
}

void Rng::set_seed(std::mt19937::result_type s) noexcept
{
  rng.seed(s);
}