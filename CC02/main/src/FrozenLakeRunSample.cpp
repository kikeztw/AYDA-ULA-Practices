#include <iostream>
#include <thread>

using namespace std::chrono_literals;

#include <FrozenLake.hpp>

int main()
{
    FrozenLake frozen_lake{};

    std::random_device rd{};
    std::mt19937 rng{rd()};

    std::uniform_int_distribution<size_t> dist{0, frozen_lake.get_action_space_size() - 1UL};

    State state = frozen_lake.create();
    
    bool terminated{false};

    std::cout << "Initial state: " << state << "\n\n";
    frozen_lake.render();
    std::this_thread::sleep_for(2s);

    while (!terminated)
    {
        Action action = Action(dist(rng));
        auto result = frozen_lake.step(action);
        state = std::get<0>(result);
        float reward = std::get<1>(result);
        terminated = std::get<2>(result);

        std::cout << "Action taken: " << action << std::endl;
        std::cout << "New state: " << state << std::endl;
        std::cout << "Reward: " << reward << "\n\n";
        std::this_thread::sleep_for(1s);
    }

    std::cout << "Terminated\n";

    std::this_thread::sleep_for(2s);

    return EXIT_SUCCESS;   
}