#include <stdexcept>

#include <FrozenLake.hpp>
#include <renderer.hpp>

FrozenLake::FrozenLake(std::optional<std::mt19937::result_type> seed) noexcept
{
    setup_renderer();
    if (seed.has_value())
    {
        initial_seed = seed.value();
    }
    else
    {
        std::random_device rd{};
        initial_seed = rd();
    }
}

FrozenLake::~FrozenLake()
{
    close();
}

State FrozenLake::create(std::optional<std::mt19937::result_type> seed) noexcept
{
    if (seed.has_value())
    {
        initial_seed = seed.value();
    }
    
    rng.seed(initial_seed);

    generate_random_environment();

    current_action = Down;
    current_reward = 0.f;

    update_renderer(&environment, num_rows, num_cols);

    return current_state;
}

size_t FrozenLake::get_action_space_size() const noexcept
{
    return size_t(NumActions);
}

size_t FrozenLake::get_observation_space_size() const noexcept
{
    return num_rows * num_cols;
}

const P& FrozenLake::get_p() const noexcept
{
    return p;
}

std::tuple<State, float, bool> FrozenLake::step(Action action) noexcept
{
    current_action = action;
    bool terminated{false};

    const auto& possibilities = p[current_state][current_action];

    float r = std::generate_canonical<float, 32>(rng);
    size_t i{0};
    float p{0.0};

    while (r > p)
    {
        r -= p;
        p = std::get<0>(possibilities[i]);
        current_state = std::get<1>(possibilities[i]);
        current_reward = std::get<2>(possibilities[i]);
        terminated = std::get<3>(possibilities[i]);
        ++i;
    }

    if (environment[current_state] == 'H')
    {
        environment[current_state] = 'C';
    }

    render();

    return std::make_tuple(current_state, current_reward, terminated);
}

void FrozenLake::render() const noexcept
{
    render_env(current_state, current_action);
}

void FrozenLake::close() const noexcept
{
    close_renderer();
}

void FrozenLake::generate_random_environment() noexcept
{
    environment.clear();
    p.clear();
    std::uniform_int_distribution<size_t> dist{4, 10};

    num_rows = dist(rng);
    num_cols = dist(rng);

    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < num_cols; ++j)
        {
            // Chance to generate a hole
            if (std::generate_canonical<float, 32>(rng) < 0.15)
            {
                environment.push_back('H');
            }
            else
            {
                environment.push_back('I');
            }
        }
    }

    std::uniform_int_distribution<size_t> col_dist{0, num_cols - 1};
    current_state = col_dist(rng);
    environment[current_state] = 'S';
    environment[(num_rows - 1) * num_cols + col_dist(rng)] = 'G';

    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < num_cols; ++j)
        {
            State left = (j == 0 ? 0 : j - 1);
            State down = (i == num_rows - 1 ? num_rows - 1 : i + 1);
            State right = (j == num_cols - 1 ? num_cols - 1 : j + 1);
            State up = (i == 0 ? 0 : i - 1);

            State state = i * num_cols + j;

            auto& state_table = p.emplace(state, StateTable{}).first->second;
            
            bool is_terminal = (environment[state] == 'H' || environment[state] == 'G');

            // If the current state is terminal, any action done will not change the state with reward 0
            if (is_terminal)
            {
                state_table[Left].push_back(std::make_tuple(1.f, state, 0.f, true));
                state_table[Down].push_back(std::make_tuple(1.f, state, 0.f, true));
                state_table[Right].push_back(std::make_tuple(1.f, state, 0.f, true));
                state_table[Up].push_back(std::make_tuple(1.f, state, 0.f, true));
            }
            else
            {
                const float probability = 1.f / 3.f;
                const State left_state = i * num_cols + left;
                const State down_state = down * num_cols + j;
                const State right_state = i * num_cols + right;
                const State up_state = up * num_cols + j;

                bool is_left_hole = environment[left_state] == 'H';
                bool is_left_goal = environment[left_state] == 'G';
                bool is_left_terminal = is_left_goal || is_left_hole;

                bool is_down_hole = environment[down_state] == 'H';
                bool is_down_goal = environment[down_state] == 'G';
                bool is_down_terminal = is_down_goal || is_down_hole;

                bool is_right_hole = environment[right_state] == 'H';
                bool is_right_goal = environment[right_state] == 'G';
                bool is_right_terminal = is_right_goal || is_right_hole;

                bool is_up_hole = environment[up_state] == 'H';
                bool is_up_goal = environment[up_state] == 'G';
                bool is_up_terminal = is_up_goal || is_up_hole;

                // The action Left will change to the left, down, or up with the same probability
                state_table[Left].push_back(
                    std::make_tuple(
                        probability,
                        left_state,
                        is_left_goal ? 1.f : 0.f, 
                        is_left_terminal
                    )
                );
                state_table[Left].push_back(
                    std::make_tuple(
                        probability,
                        down_state,
                        is_down_goal ? 1.f : 0.f, 
                        is_down_terminal
                    )
                );
                state_table[Left].push_back(
                    std::make_tuple(
                        probability,
                        up_state,
                        is_up_goal ? 1.f : 0.f, 
                        is_up_terminal
                    )
                );

                // The action Down will change to the down, left, or right with the same probability
                state_table[Down].push_back(
                    std::make_tuple(
                        probability,
                        left_state,
                        is_left_goal ? 1.f : 0.f, 
                        is_left_terminal
                    )
                );
                state_table[Down].push_back(
                    std::make_tuple(
                        probability,
                        down_state,
                        is_down_goal ? 1.f : 0.f, 
                        is_down_terminal
                    )
                );
                state_table[Down].push_back(
                    std::make_tuple(
                        probability,
                        right_state,
                        is_right_goal ? 1.f : 0.f, 
                        is_right_terminal
                    )
                );

                // The action Right will change to the right, up, or down with the same probability
                state_table[Right].push_back(
                    std::make_tuple(
                        probability,
                        up_state,
                        is_up_goal ? 1.f : 0.f, 
                        is_up_terminal
                    )
                );
                state_table[Right].push_back(
                    std::make_tuple(
                        probability,
                        down_state,
                        is_down_goal ? 1.f : 0.f, 
                        is_down_terminal
                    )
                );
                state_table[Right].push_back(
                    std::make_tuple(
                        probability,
                        right_state,
                        is_right_goal ? 1.f : 0.f, 
                        is_right_terminal
                    )
                );

                // The action Up will change to the uo, left, or right with the same probability
                state_table[Up].push_back(
                    std::make_tuple(
                        probability,
                        up_state,
                        is_up_goal ? 1.f : 0.f, 
                        is_up_terminal
                    )
                );
                state_table[Up].push_back(
                    std::make_tuple(
                        probability,
                        left_state,
                        is_left_goal ? 1.f : 0.f, 
                        is_left_terminal
                    )
                );
                state_table[Up].push_back(
                    std::make_tuple(
                        probability,
                        right_state,
                        is_right_goal ? 1.f : 0.f, 
                        is_right_terminal
                    )
                );
            }
        }
    }
}
