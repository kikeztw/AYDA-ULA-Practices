/**
 * @file Models.cpp
 * @author Alejandro Mujica (aledrums@gmail.com)
 * @copyright Copyright (c) 2022
 */
#include <algorithm>
#include <cassert>
#include <stdexcept>

#include <Models.hpp>
#include <Rng.hpp>

Good::Good() noexcept {}

Good::Good(const std::string& d) noexcept
  : description{d}, producer_processes{}, consumer_processes{} {}

Good::Good(std::string&& d) noexcept
  : description{std::forward<std::string>(d)}, producer_processes(), consumer_processes() {}

Good::Good(const Good& g) noexcept
  : description{g.description}, producer_processes{g.producer_processes}, consumer_processes{g.consumer_processes} {}

Good::Good(Good&& g) noexcept
  : Good{}
{
  swap(g);
}
  
void Good::swap(Good& g) noexcept
{
  std::swap(description, g.description);
  std::swap(producer_processes, g.producer_processes);
  std::swap(consumer_processes, g.consumer_processes);
}
  
Good& Good::operator = (Good g) noexcept
{
  swap(g);
  return *this;
}

const std::string& Good::get_description() const noexcept
{
  return description;
}

void Good::add_producer_process(std::shared_ptr<Process> p) noexcept
{
  assert(p != nullptr);
  producer_processes.push_back(p);
}

void Good::add_consumer_process(std::shared_ptr<Process> p) noexcept
{
  assert(p != nullptr);
  consumer_processes.push_back(p);
}

std::vector<std::shared_ptr<Process>>& Good::get_producer_processes() noexcept
{
  return producer_processes;
}

std::vector<std::shared_ptr<Process>>& Good::get_consumer_processes() noexcept
{
  return consumer_processes;
}
  
Process::Process() noexcept {}

/// Constructor that receives the produced good.
Process::Process(std::shared_ptr<Good> out) noexcept
  : output{out}, inputs{}, plants{} {}

/// Copy constructor.
Process::Process(const Process& p) noexcept
  : output{p.output}, inputs{p.inputs}, plants{p.plants} {}

/// Move constructor
Process::Process(Process&& p) noexcept
  : Process{} 
{ 
  swap(p); 
}

void Process::swap(Process& p) noexcept
{
  std::swap(output, p.output);
  std::swap(inputs, p.inputs);
  std::swap(plants, p.plants);
}

Process& Process::operator = (Process p) noexcept
{
  swap(p);
  return *this;
}

float Process::get_quantity(std::shared_ptr<Good> input) const
{
  auto r = inputs.find(input);
  if (r == inputs.end())
  {
    throw std::domain_error("The object is not an input");
  }
  return r->second;
}

void Process::add_input(std::shared_ptr<Good> input, float quantity)
{
  assert(input != nullptr);
  if (quantity <= 0.f)
  {
    throw std::invalid_argument("Quantity must be greater than 0");
  }
  inputs[input] = quantity;
}

void Process::add_plant(std::shared_ptr<Plant> plant) noexcept
{
  assert(plant != nullptr);
  plants.push_back(plant);
}

std::shared_ptr<Good> Process::get_output() const noexcept
{
  return output;
}

/// Gets the input table.
std::unordered_map<std::shared_ptr<Good>, float>& Process::get_inputs_table() noexcept
{
  return inputs;
}

std::vector<std::tuple<std::shared_ptr<Good>, float>> Process::get_inputs() const noexcept
{
  std::vector<std::tuple<std::shared_ptr<Good>, float>> ret_val;
  transform(inputs.begin(), inputs.end(), std::back_inserter(ret_val),
            [] (auto item) { return std::make_tuple(item.first, item.second); });
  return ret_val;
}

std::vector<std::shared_ptr<Plant>>& Process::get_plants() noexcept
{
  return plants;
}
  
Plant::Plant() noexcept {}

Plant::Plant(std::shared_ptr<Company> ow) noexcept
  : production{}, owner{ow} {}

Plant::Plant(const Plant& p) noexcept
  : production{p.production}, owner{p.owner} {}

Plant::Plant(Plant&& p) noexcept
  : Plant{}
{ 
  swap(p);
}

void Plant::swap(Plant& p) noexcept
{
  std::swap(production, p.production);
  std::swap(owner, p.owner);
}

Plant& Plant::operator = (Plant p) noexcept
{
  swap(p);
  return *this;
}

std::shared_ptr<Company> Plant::get_owner() const noexcept
{
  return owner;
}

void Plant::add_production_process(std::shared_ptr<Process> p, float q, float cap)
{
  assert(p != nullptr);
  
  if (q < 0.f)
  {
    std::invalid_argument("Quantity expected to be greater or equal to zero");
  }
    
  if (cap < 0.f || cap > 100)
  {
    std::invalid_argument("Capacity expected to be between in [0, 100]");    
  }

  production[p] = std::make_pair(q, cap);
}

void Plant::set_production_quantity(std::shared_ptr<Process> p, float q)
{
  assert(p != nullptr);
  if (q < 0.f)
  {
    std::invalid_argument("Quantity expected to be greater than zero");
  }
  
  auto r = production.find(p);
  
  if (r == production.end())
  {
    throw std::domain_error("Process does not exist");
  }
    
  r->second.first = q;
}

void Plant::inc_production_quantity(std::shared_ptr<Process> p, float q)
{
  assert(p != nullptr);

  auto r = production.find(p);
  
  if (r == production.end())
  {
    throw std::domain_error("Process does not exist");
  }
    
  r->second.first = std::max(0.f, r->second.first + q);
}

void Plant::set_production_capacity(std::shared_ptr<Process> p, float c)
{
  assert(p != nullptr);

  if (c < 0.f || c > 100)
  {
    std::invalid_argument("Capacity expected to be between in [0, 100]");    
  }
  
  auto r = production.find(p);
  
  if (r == production.end())
  {
    throw std::domain_error("Process does not exist");
  }
  
  r->second.second = c;
}

void Plant::inc_production_capacity(std::shared_ptr<Process> p, float c)
{
  assert(p != nullptr);

  auto r = production.find(p);
  
  if (r == production.end())
  {
    throw std::domain_error("Process does not exist");
  }
    
  r->second.second = std::min(100.f, std::max(0.f, r->second.second + c));
}

std::tuple<float, float> Plant::get_production(std::shared_ptr<Process> p) const
{
  assert(p != nullptr);
  
  auto r = production.find(p);
  
  if (r == production.end())
  {
    throw std::domain_error("Process does not exist");
  }
  
  return std::make_tuple(r->second.first, r->second.second);
}

std::unordered_map<std::shared_ptr<Process>, std::pair<float, float>>& Plant::get_production_table() noexcept
{
  return production;
}

std::vector<std::tuple<std::shared_ptr<Process>, float, float>> Plant::get_production() const noexcept
{
  std::vector<std::tuple<std::shared_ptr<Process>, float, float>> ret_val;
  transform(production.begin(), production.end(), std::back_inserter(ret_val),
            [] (auto item) { return std::make_tuple(item.first, item.second.first, item.second.second); });
  return ret_val;
}

Company::Company() noexcept {}

Company::Company(const std::string& r) noexcept
  : rif{r}, name{}, plants{} {}

Company::Company(std::string&& r) noexcept
  : rif{std::forward<std::string>(r)}, name{}, plants{} {}

Company::Company(const Company& c) noexcept
  : rif{c.rif}, name{c.name}, plants{c.plants} {}

Company::Company(Company&& c) noexcept
  : Company{}
{ 
  swap(c);
}

void Company::swap(Company& c) noexcept
{
  std::swap(rif, c.rif);
  std::swap(name, c.name);
  std::swap(plants, c.plants);
}

Company& Company::operator = (Company c) noexcept
{
  swap(c);
  return *this;
}

const std::string& Company::get_rif() const noexcept
{
  return rif;
}

const std::string& Company::get_name() const noexcept
{
  return name;
}

void Company::set_name(const std::string& n) noexcept
{
  name = n;
}

void Company::set_name(std::string&& n) noexcept
{
  name = move(n);
}

void Company::add_plant(std::shared_ptr<Plant> p) noexcept
{
  assert(p != nullptr);
  plants.push_back(p);
}

std::vector<std::shared_ptr<Plant>>& Company::get_plants() noexcept
{
  return plants;
}
  
std::shared_ptr<Good> ModelBuilder::create_good(const std::string& description) noexcept
{
  return std::make_shared<Good>(description);
}

std::shared_ptr<Process> ModelBuilder::create_process(std::shared_ptr<Good> output, std::vector<std::tuple<std::shared_ptr<Good>, float>>& inputs) noexcept
{
  auto p = std::make_shared<Process>(output);
  output->add_producer_process(p);
  
  for (auto input: inputs)
    {
      p->add_input(std::get<0>(input), std::get<1>(input));
      std::get<0>(input)->add_consumer_process(p);
    }
  
  return p;
}

std::shared_ptr<Process> ModelBuilder::create_process_randomly(std::shared_ptr<Good> output, std::unordered_set<std::shared_ptr<Good>>& possible_inputs) noexcept
{
  auto p = std::make_shared<Process>(output);
  output->add_producer_process(p);
  
  constexpr float prob = 0.85;
  
  for (auto input: possible_inputs)
    {
      if (Rng::flip_coin(prob))
        {
          p->add_input(input, Rng::random_uniform_real(0.1f, 0.3f));
          input->add_consumer_process(p);
        }
    }
  
  return p;
}

std::shared_ptr<Plant> ModelBuilder::create_plant(std::shared_ptr<Company> owner, std::vector<std::tuple<std::shared_ptr<Process>, float, float>>& production) noexcept
{
  auto p = std::make_shared<Plant>(owner);
  owner->add_plant(p);
  
  for (auto pr: production)
    {
      std::get<0>(pr)->add_plant(p);
      p->add_production_process(std::get<0>(pr), std::get<1>(pr), std::get<2>(pr));
    }
  
  return p;
}

std::shared_ptr<Plant> ModelBuilder::create_plant_randomly(std::shared_ptr<Company> owner, size_t num_processes, std::vector<std::shared_ptr<Process>>& process_set) noexcept
{
  auto p = std::make_shared<Plant>(owner);
  owner->add_plant(p);
  
  std::unordered_set<std::shared_ptr<Good>> produced_goods_bag;
  
  for (unsigned int k = 0; k < num_processes; ++k)
    {
      auto pr = *Rng::random_choice(process_set.begin(), process_set.end());
      
      if (produced_goods_bag.find(pr->get_output()) != produced_goods_bag.end())
        {
          continue;
        }
              
      produced_goods_bag.insert(pr->get_output());
      
      pr->add_plant(p);
      p->add_production_process(pr, 0, Rng::random_uniform_real(70.f, 100.f));
    }
  
  return p;
}

std::shared_ptr<Company> ModelBuilder::create_company(const std::string& rif) noexcept
{
  return std::make_shared<Company>(rif);
}

ProductionData::ProductionData(ProductionData&& pd) noexcept
  : ProductionData{}
{ 
  swap(pd);
}

ProductionData& ProductionData::operator = (ProductionData pd) noexcept
{
  swap(pd);
  return *this;
}

void ProductionData::swap(ProductionData& pd) noexcept
{
  std::swap(goods, pd.goods);
  std::swap(processes, pd.processes);
  std::swap(plants, pd.plants);
  std::swap(companies, pd.companies);
  std::swap(relationships, pd.relationships);
}      
