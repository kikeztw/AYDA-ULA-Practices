#pragma once

#include <optional>
#include <random>

#include <types.hpp>

class ValueIteration
{
public:
    using ResultType = std::tuple<std::vector<float>, std::vector<float>>;

    ValueIteration(size_t _num_states, size_t _num_actions, const P& _p, float _gamma) noexcept;

    Action get_action(size_t state) const noexcept; 

    ResultType solve(size_t num_iterations, float theta) noexcept;

private:
    size_t num_states;
    size_t num_actions;
    const P& p;
    float gamma;
    
    std::vector<float> values;
    std::vector<float> policy;
};

class PolicyIteration
{
public:
    using ResultType = std::tuple<std::vector<float>, std::vector<std::vector<float>>>;

    PolicyIteration(size_t _num_states, size_t _num_actions, const P& _p, float _gamma,
                    std::optional<std::mt19937::result_type> seed = std::nullopt) noexcept;

    Action get_action(size_t state) const noexcept; 

    ResultType solve(size_t num_iterations, float theta) noexcept;

private:
    size_t num_states;
    size_t num_actions;
    const P& p;
    float gamma;
    std::mt19937::result_type initial_seed;
    std::mt19937 rng;
    
    std::vector<float> values;
    std::vector<std::vector<float>> policy;
};
