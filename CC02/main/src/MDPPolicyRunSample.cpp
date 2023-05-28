#include <iostream>
#include <thread>

using namespace std::chrono_literals;

#include <FrozenLake.hpp>
#include <MDP.hpp>

int main()
{
    FrozenLake env{};
    State state = env.create();

    PolicyIteration agent{env.get_observation_space_size(), env.get_action_space_size(), env.get_p(), 0.9};
    // agent.solve(100000, 0.1);   
    agent.solve(100, 0.1);     
    bool terminated = false;

    env.render();
    std::this_thread::sleep_for(2s);

    while (!terminated)
    {
        Action action = agent.get_action(state);
        auto result = env.step(action);
        state = std::get<0>(result);
        float reward = std::get<1>(result);
        terminated = std::get<2>(result);
        std::cout << "Action taken: " << action << std::endl;
        std::cout << "New state: " << state << std::endl;
        std::cout << "Reward: " << reward << "\n\n";
        std::this_thread::sleep_for(1s);
    }

    std::this_thread::sleep_for(2s);
    env.close();

    return EXIT_SUCCESS;
}
