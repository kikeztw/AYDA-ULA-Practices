#include <iostream>
#include <algorithm>
#include <FrozenLake.hpp>
#include <MDP.hpp>
#include <tuple>

float abs(float temp_value, float max){
    float diff = temp_value - max;
    return diff < 0 ? -1 * (diff) : diff;
}

ValueIteration::ValueIteration(size_t _num_states, size_t _num_actions, const P& _p, float _gamma) noexcept
    : num_states{_num_states}, num_actions{_num_actions}, p{_p}, gamma{_gamma},
      values(num_states, 0.f), policy(num_states, 0.f)
{

}

Action ValueIteration::get_action(size_t state) const noexcept
{
    return Action(size_t(policy[state]));
}

ValueIteration::ResultType ValueIteration::solve(size_t num_iterations, float theta) noexcept
{
    for (size_t num_i = 0; num_i < num_iterations; num_i++){ // iteraciones
       	float delta = 0;
        for (size_t state = 0; state <  num_states; state++) // estados
        {
            float max = 0;
            float max_a = 0;
			float temp_value = values[state];
            for (int action = 0; action < Action::NumActions; action++) //acciones
            {
                float sum = 0; 
                const auto& tuple_vector = p.at(state).at((Action) action);
                for(auto tuple : tuple_vector){
                    sum += std::get<0>(tuple) * (std::get<2>(tuple) + gamma * values[std::get<1>(tuple)]);
                }
                if(sum > max){
                    max = sum;
                    max_a = action;
                }
            }
            delta = std::max(delta, abs(temp_value, max)); 
            values[state] = max;
            policy[state] = max_a;
        }
        if(delta < theta){
            break;
        }
    }
    return std::make_tuple(values, policy);
}

PolicyIteration::PolicyIteration(size_t _num_states, size_t _num_actions, const P& _p, float _gamma,
                                 std::optional<std::mt19937::result_type> seed) noexcept
    : num_states{_num_states}, num_actions{_num_actions}, p{_p}, gamma{_gamma},
      values(num_states, 0.f), policy(num_states, std::vector<float>(num_actions))
{
    if (seed.has_value())
    {
        initial_seed = seed.value();
    }
    else
    {
        std::random_device rd{};
        initial_seed = rd();
    }

    rng.seed(initial_seed);

    std::uniform_int_distribution<size_t> action_dist{0UL, num_actions - 1};

    // random deterministic policy
    for (auto& ps: policy)
    {
        ps[action_dist(rng)] = 1.f;
    }
}

Action PolicyIteration::get_action(size_t state) const noexcept
{
    const auto& current_action_prob_dist = policy[state];

    size_t action = 0;
    
    for ( ; action < current_action_prob_dist.size(); ++action)
    {
        if (current_action_prob_dist[action] == 1.f)
        {
            break;
        }
    }

    return Action(action);
}

PolicyIteration::ResultType PolicyIteration::solve(size_t num_iterations, float theta) noexcept
{

    for (size_t num_i = 0; num_i < num_iterations; num_i++)
    {
        while (true)
        {
            float delta = 0;
            for (size_t state = 0; state <  num_states; state++) // estados
            {
                float temp_value = values[state];
                for (int action = 0; action < Action::NumActions; action++) //acciones
                {
                    float sum = 0; 
                    const auto& tuple_vector = p.at(state).at((Action) action);
                    for(auto tuple : tuple_vector){
                        sum += std::get<0>(tuple) * (std::get<2>(tuple) + gamma * values[std::get<1>(tuple)]);
                    }
                    // std::cout<<" Suma Result"<<sum<<"- ";
                    values[state] = sum;
                }
                delta = std::max(delta, abs(temp_value, values[state])); 
              
            }
            if(delta < theta){
                break;
            }
        }
        
        bool policy_stable = true;
        for (size_t state = 0; state < num_states; state++)
        {
            float max = 0;
            int max_a = -1;
            auto old_action = std::distance(policy[state].begin(), std::max_element(std::begin(policy[state]), std::end(policy[state])));
            std::cout<<"old_action"<<old_action<<"--";
            for (int action = 0; action < Action::NumActions; action++) //acciones
            {
                float sum = 0; 
                const auto& tuple_vector = p.at(state).at((Action) action);
                for(auto tuple : tuple_vector){
                    sum += std::get<0>(tuple) * (std::get<2>(tuple) + gamma * values[std::get<1>(tuple)]);
                }
                if(sum > max){
                    max = sum;
                    max_a = action;
                }
            }

            if(old_action != max_a){
                policy_stable = false;
            }

            policy[state].clear();
            policy[state].push_back(policy[state][max_a]);
        }

        if(policy_stable){
            std::cout<<"break";
            break;
        }
   }
    return std::make_tuple(values, policy);
}
