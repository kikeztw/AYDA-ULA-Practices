#include <iostream>

#include <FrozenLake.hpp>


int main()
{
    FrozenLake frozen_lake{};
    frozen_lake.create();

    std::cout << "Number of actions: " << frozen_lake.get_action_space_size() << std::endl;
    std::cout << "Number of states: " << frozen_lake.get_observation_space_size() << std::endl;

    const P& p_table = frozen_lake.get_p();

    std::cout << std::boolalpha;

    std::cout << "P = {\n";

    // Iterate over the pairs (state, action_table)
    for (const auto& state_pair: p_table)
    {
        // Print the state value
        std::cout << "    " << state_pair.first << ": {\n";
        
        // Iterate over the pairs (action, possibilities)
        for (const auto& action_pair: state_pair.second)
        {
            // Print the action value
            std::cout << "        " << action_pair.first << ": {\n";

            // Iterate over possibilities of transition list
            for (const auto& possibility: action_pair.second)
            {
                std::cout << "            ("
                          << std::get<0>(possibility) << ", "
                          << std::get<1>(possibility) << ", "
                          << std::get<2>(possibility) << ", "
                          << std::get<3>(possibility) << ")\n";
            }
            std::cout << "        }\n";
        }
        std::cout << "    }\n";
    }
    
    std::cout << "}\n";

    return EXIT_SUCCESS;
}
