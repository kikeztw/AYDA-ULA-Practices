/**
 * @file Models.hpp
 * @author Alejandro Mujica (aledrums@gmail.com)
 * @copyright Copyright (c) 2022
 */
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Forward declaration for models.
class Process;
class Plant;
class Company;

/// Model for goods or services.
class Good
{
public:
  /// Default constructor. Don't use it.
  Good() noexcept;

  /// Constructor that receives a description as l-value.
  Good(const std::string& d) noexcept;

  /// Constructor that receives a description as r-value.
  Good(std::string&& d) noexcept;

  /// Copy constructor.
  Good(const Good& g) noexcept;

  /// Move contructor.
  Good(Good&& g) noexcept;
  
  /// Swap elements.
  void swap(Good& g) noexcept;
  
  /// Assignment operator.
  Good& operator = (Good g) noexcept;

  /// Gets the description.
  const std::string& get_description() const noexcept;

  /// Add a process as a producer of this good.
  void add_producer_process(std::shared_ptr<Process> p) noexcept;

  /// Add a process as a consumer of this good.
  void add_consumer_process(std::shared_ptr<Process> p) noexcept;

  /// Returns the list of producer processes.
  std::vector<std::shared_ptr<Process>>& get_producer_processes() noexcept;

  /// Returns the list of consumer processes.
  std::vector<std::shared_ptr<Process>>& get_consumer_processes() noexcept;
  
private:
  std::string description;
  std::vector<std::shared_ptr<Process>> producer_processes;
  std::vector<std::shared_ptr<Process>> consumer_processes;
};

/// Represents a formula to generate a good.
class Process
{
public:
  /// Default constructor, Don't use it.
  Process() noexcept;
  
  /// Constructor that receives the produced good.
  Process(std::shared_ptr<Good> out) noexcept;

  /// Copy constructor.
  Process(const Process& p) noexcept;

  /// Move constructor
  Process(Process&& p) noexcept;
  
  /// Swap elements.
  void swap(Process& p) noexcept;

  /// Assignment operator.
  Process& operator = (Process p) noexcept;

  /** 
   * Returns the required amount of an input to produce
   * one unit of the output.
   *
   * @throw std::domain_error if the input does not belong to the process.
   */
  float get_quantity(std::shared_ptr<Good> input) const;

  /**
   * Add a new input associated with the required amount.
   * If the input already exists, then the required amount is updated.
   *      
   * @throw std::invalid_argument if quantity less or equal to zero.
   */
  void add_input(std::shared_ptr<Good> input, float quantity);

  /// Add a new plant that owns this process.
  void add_plant(std::shared_ptr<Plant> plant) noexcept;

  /// Gets the produced good.
  std::shared_ptr<Good> get_output() const noexcept;

  /// Gets the input table.
  std::unordered_map<std::shared_ptr<Good>, float>& get_inputs_table() noexcept;
  
  /// Returns a list of tuples (good, quantity) of the inputs.
  std::vector<std::tuple<std::shared_ptr<Good>, float>> get_inputs() const noexcept;

  /// Returns the list of plants that own this process.
  std::vector<std::shared_ptr<Plant>>& get_plants() noexcept;
  
private:
  std::shared_ptr<Good> output;
  std::unordered_map<std::shared_ptr<Good>, float> inputs;
  std::vector<std::shared_ptr<Plant>> plants;
};

/** 
 * Represents a plant or a company branch office that produces goods.
 * 
 * The goods that produces are defined by each process that the plant owns.
 *
 * In this model, the production of the goods is represented by a table of processes,
 * each of them associated with the actual production and its capacity of production
 * represented as a percentage.
 *
 * For instance: Lets P1 be a process owned by a plant. Suppose that P1
 * has a capacity of production of 100 units by time unit and in a time point
 * it is producing 80 units, then P1 will be associated with the pair (80, 80).
 * That means that P1 is producing 80 units of its output and it is using the 80\%
 * of its capacity of production.
 */
class Plant
{
public:
  /// Default constructor. Don't use it.
  Plant() noexcept;

  /// Constructor with the owner company.
  Plant(std::shared_ptr<Company> ow) noexcept;

  /// Copy constructor.
  Plant(const Plant& p) noexcept;

  /// Move constructor.
  Plant(Plant&& p) noexcept;
  
  /// Swap elements.
  void swap(Plant& p) noexcept;

  /// Assignment operator.
  Plant& operator = (Plant p) noexcept;

  /// Gets the owner company.
  std::shared_ptr<Company> get_owner() const noexcept;

  /**
   * Adds a process with its actual production and its capacity of production.
   *
   * If the process already exists, then the production and the capacity will be updated.
   *
   * @throw std::invalid_argument if the quantity or the capacity are less than zero
   * or the capacity is greater than 100.
   */
  void add_production_process(std::shared_ptr<Process> p, float q, float cap);

  /**
   * Sets a new quantity of production to a process.
   *
   * @throw std::domain_error if the process does not exist.
   * @throw std::invalid_argument if the quantity is less than zero.
   */
  void set_production_quantity(std::shared_ptr<Process> p, float q);

  /**
   * Increments the quantity of production of a given process.
   * A negative value will decrement the quantity of production.
   * The value will be truncated to zero.
   *
   * @throw std::domain_error if the process does not exist.
   */
  void inc_production_quantity(std::shared_ptr<Process> p, float q);

  /**
   * Sets a new capacity of production to a process.
   *
   * @throw std::domain_error if the process does not exist.
   * @throw std::invalid_argument if the quantity is not in [0, 100].
   */
  void set_production_capacity(std::shared_ptr<Process> p, float c);

  /**
   * Increments the capacity of production of a given process.
   * A negative value will decrement the capacity of production.
   * The value will be truncated to be in [0, 100].
   *
   * @throw std::domain_error if the process does not exist.
   */
  void inc_production_capacity(std::shared_ptr<Process> p, float c);

  /** 
   * Gets a tuple (q, c) with the quantity and capacity of production
   * of a given process.
   *
   * @throw std::domain_error if the process does not exist.
   */
  std::tuple<float, float> get_production(std::shared_ptr<Process> p) const;

  /// Gets the table of production.
  std::unordered_map<std::shared_ptr<Process>, std::pair<float, float>>& get_production_table() noexcept;
  
  /// Gets a list of tuples (process, quantity, capacity) de los procesos
  std::vector<std::tuple<std::shared_ptr<Process>, float, float>> get_production() const noexcept;

private:
  std::unordered_map<std::shared_ptr<Process>, std::pair<float, float>> production;
  std::shared_ptr<Company> owner;
};

/// Model for a company. 
class Company
{
public:
  /// Default constructor. Don't use it.
  Company() noexcept;
  
  /// Constructor with a R.I.F as l-value.
  Company(const std::string& r) noexcept;

  /// Constructor with a R.I.F as l-value.
  Company(std::string&& r) noexcept;

  /// Copy constructor.
  Company(const Company& c) noexcept;

  /// Move constructor.
  Company(Company&& c) noexcept;
  
  /// Swap elements.
  void swap(Company& c) noexcept;

  /// Assignment operator.
  Company& operator = (Company c) noexcept;

  /// Gets the R.I.F.
  const std::string& get_rif() const noexcept;

  /// Gets the name.
  const std::string& get_name() const noexcept;

  /// Sets name as l-value.
  void set_name(const std::string& n) noexcept;

  /// Sets name as r-value.
  void set_name(std::string&& n) noexcept;

  /// Adds the plant p.
  void add_plant(std::shared_ptr<Plant> p) noexcept;

  /// Returns the list of plants.
  std::vector<std::shared_ptr<Plant>>& get_plants() noexcept;
  
private:
  std::string rif;
  std::string name;
  std::vector<std::shared_ptr<Plant>> plants;
};

/**
 * This class is intended to wrap the complex construction of some of the objects by granting that they 
 * are constructed with the right references in both directions.
 */
class ModelBuilder
{
public:
  /**
   * @brief Creates a new good.
   * 
   * @param description: The description (name) of the good.
   * @return a shared pointer to the good.
   */
  std::shared_ptr<Good> create_good(const std::string& description) noexcept;

  /**
   * @brief Creates a process.
   * 
   * @param output: The produced good. 
   * @param inputs: A list of tuples (good, amount) with the inputs.
   * @return a shared pointer to the process.  
   */
  std::shared_ptr<Process> create_process(std::shared_ptr<Good> output, std::vector<std::tuple<std::shared_ptr<Good>, float>>& inputs) noexcept;
  
  /**
   * @brief Creates a process with random inputs.
   * 
   * This method creates a process by selecting inputs and their required amounts randomly.
   * 
   * @param output: The good that the process produces.
   * @param possible_inputs: A set with the possible inputs of the process. 
   * @return a shared pointer to the process. 
   */
  std::shared_ptr<Process> create_process_randomly(std::shared_ptr<Good> output, std::unordered_set<std::shared_ptr<Good>>& possible_inputs) noexcept;

  /**
   * @brief Creates a plant.
   * 
   * @param owner: The company to which the plant belongs.
   * @param production: A list of tuples (process, produced amount, used capacity) with all of the production processes.
   * @return a shared pointer to the plant. 
   */
  std::shared_ptr<Plant> create_plant(std::shared_ptr<Company> owner, std::vector<std::tuple<std::shared_ptr<Process>, float, float>>& production) noexcept;
  
  /**
   * @brief Creates a plant with random processes.
   * 
   * @param owner: The company to which the plant belongs.
   * @param num_processes: The maximum amount of processes that the plant will have.
   * @param process_set: A set of available processes. 
   * @return a shared pointer to the plant. 
   */
  std::shared_ptr<Plant> create_plant_randomly(std::shared_ptr<Company> owner, size_t num_processes, std::vector<std::shared_ptr<Process>>& process_set) noexcept;
  
  /**
   * @brief Creates a company.
   * 
   * @param rif: The R.I.F. 
   * @return A shared pointer to the company.
   */
  std::shared_ptr<Company> create_company(const std::string& rif) noexcept;
};

/** 
 * This class contains all of the goods, processes, plants, and companies
 * of a production network.
 */
class ProductionData
{
public:
  std::unordered_set<std::shared_ptr<Good>> goods;
  std::unordered_set<std::shared_ptr<Process>> processes;
  std::unordered_set<std::shared_ptr<Plant>> plants;
  std::unordered_set<std::shared_ptr<Company>> companies;

  /// Represents a relationship (sale - purchase) between two plants.
  using RelationShip = std::tuple<std::shared_ptr<Plant>, // Seller
                                  std::shared_ptr<Plant>, // Buyer
                                  std::shared_ptr<Good>,  // The good
                                  float,                  // Sold quantity
                                  float>;                 // Unitarian price

  /// List of relationships.
  std::vector<RelationShip> relationships;

  /// Default constructor.
  ProductionData() = default;

  /// Deleted copy constructor.
  ProductionData(const ProductionData&) = delete;

  /// Move constructor.
  ProductionData(ProductionData&& pd) noexcept;

  /// Assignment operator.
  ProductionData& operator = (ProductionData pd) noexcept;
  
  /// Swap elements.
  void swap(ProductionData& pd) noexcept;
};

