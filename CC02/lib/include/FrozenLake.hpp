#pragma once

#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include <types.hpp>

class FrozenLake
{
public:
    FrozenLake(std::optional<std::mt19937::result_type> seed = std::nullopt) noexcept;

    ~FrozenLake();

    State create(std::optional<std::mt19937::result_type> seed = std::nullopt) noexcept;

    size_t get_action_space_size() const noexcept;

    size_t get_observation_space_size() const noexcept;

    const P& get_p() const noexcept;

    std::tuple<State, float, bool> step(Action action) noexcept;

    void render() const noexcept;

    void close() const noexcept;

private:
    void generate_random_environment() noexcept;

    std::mt19937::result_type initial_seed;
    std::mt19937 rng;
    size_t num_rows;
    size_t num_cols;
    std::vector<char> environment;
    P p;
    
    State current_state;
    Action current_action;
    float current_reward;
};
