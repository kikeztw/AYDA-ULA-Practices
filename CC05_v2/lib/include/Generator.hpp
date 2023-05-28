/**
 * @file Generator.hpp
 * @author Alejandro Mujica (aledrums@gmail.com)
 * @copyright Copyright (c) 2022
 */
#pragma once

class ProductionData;

class NetworkGenerator
{
public:
  /**
   * @brief Generates a random production network.
   * 
   * This function generates a very idealist-random-production network. The features that this function provides are:
   * 
   * - It is structured by production levels.
   * - Each plant is only in one of the levels (although many plants might belong to the same company).
   * - Each good is produced in an only level too.
   * - Each good has a unique producer process.
   * - The inputs of each process are produced in any of the previous levels.
   * - The processes in the first level do not have inputs.
   * - The production amounts in the last level were generated randomly.
   * - The production amounts of the non-last-level-goods are computed to satisfy the required production.
   * - The production prices in the first level were generated randomly.
   * - The production prices of the non-first-level-goods are computed according to the input prices.
   * 
   * @param num_levels: The number of production levels in the network.
   * @param num_plants_per_level: Expected number of plants by level.
   * @param num_goods_per_plant: Expected number of goods produced by a plant.
   * @param num_inputs_per_process: Expected number of inputs for a process.
   * @param probability_to_repeat_good: Probability of many plants producing the same good.
   * @param probability_to_repeat_company: Probability of many plants belonging to the same company.
   * @return ProductionData 
   */
  static ProductionData generate_random_production_data(unsigned int num_levels,
                                                        unsigned int num_plants_per_level,
                                                        unsigned int num_goods_per_plant,
                                                        unsigned int num_inputs_per_process,
                                                        float probability_to_repeat_good,
                                                        float probability_to_repeat_company);
};
