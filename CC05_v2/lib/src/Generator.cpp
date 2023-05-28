/**
 * @file Generator.cpp
 * @author Alejandro Mujica (aledrums@gmail.com)
 * @copyright Copyright (c) 2022
 */
#include <cassert>
#include <functional>

#include <Generator.hpp>
#include <Models.hpp>
#include <Rng.hpp>

ProductionData NetworkGenerator::generate_random_production_data(unsigned int num_levels,
                                                                 unsigned int num_plants_per_level,
                                                                 unsigned int num_goods_per_plant,
                                                                 unsigned int num_inputs_per_process,
                                                                 float probability_to_repeat_good,
                                                                 float probability_to_repeat_company)
  {
    ProductionData pd;
    ModelBuilder builder;

    std::vector<std::vector<std::shared_ptr<Plant>>> levels{num_levels, std::vector<std::shared_ptr<Plant>>{}};
    std::vector<std::unordered_set<std::shared_ptr<Process>>> processes_per_level{num_levels, std::unordered_set<std::shared_ptr<Process>>{}};
    unsigned int good_counter{0};

    for (unsigned int l = 0; l < num_levels; ++l)
      {
        auto& level = levels[l];

        // Generating num plants for level l.
        unsigned int num_plants = std::max(1U, (unsigned int) Rng::random_normal<float>(num_plants_per_level, 2.f));

        std::unordered_set<std::shared_ptr<Process>> processes_at_this_level;

        for (unsigned int i = 0; i < num_plants; ++i)
          {
            // Selecting goods for the plant.
            unsigned int num_goods = std::max(1U, (unsigned int) Rng::random_normal<float>(num_goods_per_plant, 1.f));

            std::vector<std::tuple<std::shared_ptr<Process>, float, float>> production;

            for (auto j = 0; j < num_goods; ++j)
              {
                bool repeat_good = processes_at_this_level.size() > 0 && Rng::flip_coin(probability_to_repeat_good);

                if (repeat_good)
                  {
                    auto process = *Rng::random_choice(processes_at_this_level.begin(), processes_at_this_level.end());

                    // 0 production (for now) and random used capacity.
                    production.push_back(std::make_tuple(process, 0, Rng::random_uniform_real(70.f, 100.f)));
                  }
                else
                  {
                    auto good = builder.create_good(std::string("G") + std::to_string(++good_counter));
                    pd.goods.insert(good);

                    std::unordered_set<std::shared_ptr<Good>> input_bag;
                    std::vector<std::tuple<std::shared_ptr<Good>, float>> inputs;

                    unsigned int num_inputs = l == 0 ? 0 : std::max(1U, (unsigned int) Rng::random_normal<float>(num_inputs_per_process, 1.f));

                    for (unsigned int k = 0; k < num_inputs; ++k)
                      {
                        assert(l > 0);
                        unsigned int input_level = Rng::random_uniform_int(0U, l - 1U);

                        assert(input_level < l);
                        assert(processes_per_level[input_level].size() > 0);
                        auto input = (*Rng::random_choice(processes_per_level[input_level].begin(), processes_per_level[input_level].end()))->get_output();

                        if (input_bag.find(input) != input_bag.end())
                          {
                            continue;
                          }
                      
                        input_bag.insert(input);
                        inputs.push_back(std::make_tuple(input, Rng::random_uniform_real(0.1f, 2.f)));
                      }

                    auto process = builder.create_process(good, inputs);
                    pd.processes.insert(process);
                    processes_per_level[l].insert(process);
                    assert(processes_per_level[l].size() > 0);
                    production.push_back(std::make_tuple(process, 0, Rng::random_uniform_real(70.f, 100.f)));
                  }
              }
            
            bool repeat_company = pd.companies.size() > 0 && Rng::flip_coin(probability_to_repeat_company);
            auto owner = repeat_company ? *Rng::random_choice(pd.companies.begin(), pd.companies.end()) : builder.create_company(Rng::random_rif());
            pd.companies.insert(owner);

            auto plant = builder.create_plant(owner, production);
            pd.plants.insert(plant);
            level.push_back(plant);

            for (auto pr: production)
              {
                processes_at_this_level.insert(std::get<0>(pr));
              }
          }
      }

    // 30% of fixed earning to all production.
    constexpr float fixed_earning = 1.3; 

    // Set production costs
    std::unordered_map<std::shared_ptr<Process>, float> production_costs;
    
    // Set random production costs to the processes at the first leve
    for (auto process: processes_per_level[0])
      {
        assert(process->get_inputs_table().empty());
        float production_cost = (float) Rng::random_uniform_int(40, 70);
        production_costs[process] = production_cost;
      }

    // Set prices for the other processes according to the inputs.
    for (unsigned int process_level_index = 1; process_level_index < processes_per_level.size(); ++process_level_index)
      {
        for (auto process: processes_per_level[process_level_index])
          {
            float production_cost = 0.f;

            // Compute the production cost. It adds the fixed earning to each input cost.
            for (auto& input: process->get_inputs_table())
              {
                auto input_price = production_costs.find(input.first->get_producer_processes()[0])->second * fixed_earning * input.second;
                production_cost += input_price;
              }

            production_costs[process] = production_cost;
          }
      }

    // Set production by goods (using the processes by level).

    // Table to store the required production by process.
    std::unordered_map<std::shared_ptr<Process>, float> required_production_table;

    // For the very last level, the production is set randomly.
    for (auto process: processes_per_level.back())
      {
        required_production_table[process] = Rng::random_uniform_real(5.f, 10.f) * 100.f;
      }
    
    // For each level, in reverse, compute the amount of required inputs.
    for (unsigned int i = processes_per_level.size(); i > 0; --i)
      {
        auto& level = processes_per_level[i - 1];

        for (auto process: level)
          {
            auto found = required_production_table.find(process);

            // It is possible that not all of the goods are in the required production table because they might not be used as inputs.
            if (found ==  required_production_table.end())
              {
                continue;
              }

            auto required_production = found->second;

            // Compute and add the required input amounts.
            for (auto p: process->get_inputs_table())
              {
                required_production_table.emplace(p.first->get_producer_processes()[0], 0.f).first->second += required_production * p.second;
              }
          }
      }

    // Table to store what plants produce what goods and how much they produce.
    std::unordered_map<std::shared_ptr<Good>, std::unordered_map<std::shared_ptr<Plant>, float>> good_production_by_plants;

    // Set the production amount to each plant by process.
    for (auto p: required_production_table)
      {
        auto process = p.first;
        auto required_amount = p.second;
        auto remaining_production = required_amount;

        std::vector<float> production_proportion_by_plant;

        // Set proportions for all the plants - 1.
        for (unsigned int i = 0; i < process->get_plants().size() - 1; ++i)
          {
            production_proportion_by_plant.push_back(Rng::random_uniform());
          }

        auto plants_iterator = process->get_plants().begin();

        for (auto proportion: production_proportion_by_plant)
          {
            auto amount_to_produce = remaining_production * proportion;
            (*plants_iterator)->set_production_quantity(process, amount_to_produce);

            // Subtract amount to produce to remaining production.
            remaining_production -= amount_to_produce;

            good_production_by_plants.emplace(process->get_output(), std::unordered_map<std::shared_ptr<Plant>, float>{}).first->second[*plants_iterator] = amount_to_produce;

            ++plants_iterator;
          }

        assert(plants_iterator != process->get_plants().end());
        assert(remaining_production > 0.f);

        // The last plant of the list will produce the remaining amount.
        (*plants_iterator)->set_production_quantity(process, remaining_production);
        good_production_by_plants.emplace(process->get_output(), std::unordered_map<std::shared_ptr<Plant>, float>{}).first->second[*plants_iterator] = remaining_production;
      }

    // Table to avoid redundance in the relationships.
    using RelKey = std::tuple<std::shared_ptr<Plant>, std::shared_ptr<Plant>, std::shared_ptr<Good>>;

    struct RelKeyHash
    {
      size_t operator () (const RelKey& t) const noexcept
      {
        size_t seed{0};
        seed ^= std::hash<std::shared_ptr<Plant>>{}(std::get<0>(t)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<std::shared_ptr<Plant>>{}(std::get<1>(t)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<std::shared_ptr<Good>>{}(std::get<2>(t)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
      }
    };

    struct RelKeyEqual
    {
      bool operator () (const RelKey& k1, const RelKey& k2) const noexcept
      {
        return std::get<0>(k1) == std::get<0>(k2) && std::get<1>(k1) == std::get<1>(k2) && std::get<2>(k1) == std::get<2>(k2);
      }
    };

    std::unordered_map<RelKey, ProductionData::RelationShip*, RelKeyHash, RelKeyEqual> rel_table;

    // Generate relationships by level (on plants) in reverse.
    // Level 0 won't generate relationships.
    for (unsigned int i = levels.size(); i > 1; --i)
      {
        auto& level = levels[i - 1];

        for (auto plant: level)
          {
            for (auto p: plant->get_production_table())
              {
                auto process = p.first;
                auto required_production = p.second.first;

                // Look for seller for each input.
                for (auto input: process->get_inputs_table())
                  {
                    auto input_good = input.first;
                    auto required_amount = input.second * required_production;

                    auto& input_producers = good_production_by_plants[input_good];

                    for (auto& plant_production: input_producers)
                      {
                        if (required_amount <= 0.f || plant_production.second <= 0.f)
                          {
                            continue;
                          }

                        auto purchase_amount = std::min(required_amount, plant_production.second);
                        
                        // Subtract the purchase amount from the plant production and the required amount.
                        plant_production.second -= purchase_amount;
                        required_amount -= purchase_amount;

                        auto found_rel = rel_table.find(std::make_tuple(plant_production.first, plant, input_good));

                        if (found_rel == rel_table.end())
                        {
                          // Generate the relationship
                          pd.relationships.push_back(
                            std::make_tuple(
                              plant_production.first, // the seller
                              plant, // the buyer
                              input_good, // The good
                              purchase_amount, // The amount sold.
                              production_costs[input_good->get_producer_processes()[0]]*fixed_earning // The price.
                            )
                          );
                          rel_table[std::make_tuple(plant_production.first, plant, input_good)] = &pd.relationships.back();
                        }
                        else
                        {
                          auto rel = found_rel->second;
                          std::get<3>(*rel) += purchase_amount;
                        }
                      }
                  }
              }
          }
      }
  
    return pd;
  }