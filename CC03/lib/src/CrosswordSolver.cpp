#include <algorithm>
#include <iostream>
#include <CrosswordSolver.hpp>
#include <unordered_set>

#include <NoImplementedError.hpp>

CrosswordSolver::CrosswordSolver(const Crossword& _crossword) noexcept
  : crossword{_crossword}, domains{}
{
  for (const Variable& v: crossword.get_variables())
  {
    domains.emplace(&v, crossword.get_words());
  }
}

void CrosswordSolver::print(const CrosswordSolver::AssignmentT& assignment, std::ostream& out) const noexcept
{
  auto letters = letter_grid(assignment);

  for (size_t i = 0; i < letters.size(); ++i)
  {
    for (size_t j = 0; j < letters[i].size(); ++j)
    {
      if (letters[i][j] == ' ')
      {
        out << "█";
      }
      else
      {
        out << letters[i][j];
      }
    }
    out << "\n";
  }
}

std::vector<std::vector<char>> CrosswordSolver::letter_grid(const CrosswordSolver::AssignmentT& assignment) const noexcept
{
  std::vector<std::vector<char>> result{crossword.height(), std::vector<char>(crossword.width(), ' ')};

  for (const auto& p: assignment)
  {
    auto dir = p.first->get_direction();

    for (size_t k = 0; k < p.second.size(); ++k)
    {
      auto pos = p.first->get_position();
      size_t i = pos.first + (dir == Variable::direction::down ? k : 0);
      size_t j = pos.second + (dir == Variable::direction::across ? k : 0);
      result[i][j] = p.second[k];
    }
  }

  return result;
}

std::optional<CrosswordSolver::AssignmentT> CrosswordSolver::solve() noexcept
{
  enforce_node_consistency();
  ac3();
  return backtrack(CrosswordSolver::AssignmentT{});
}

void CrosswordSolver::enforce_node_consistency() noexcept
{
  std::unordered_map<const Variable*, Crossword::WordSetT, Variable::PointerHash, Variable::PointerEqual> domains_aux;
  for(auto& _domain : domains){
    auto& _word_list = _domain.second;
    auto& _variable = _domain.first;
    Crossword::WordSetT _word_list_aux;
    for(auto& _word: _word_list){
      if(_word.length() == _variable->get_cells().size()){
        _word_list_aux.insert(_word);
      }
    }
    domains_aux[_variable] = _word_list_aux;
  }
  domains = domains_aux;
}

bool CrosswordSolver::revise(const Variable* x, const Variable* y) noexcept
{
  auto& domain_x = domains.at(x);
  auto& domain_y = domains.at(y);
  auto& _overlap = crossword.get_overlaps().at(std::make_pair(x, y));

  if(!_overlap.has_value()){
    return false;
  }

  Crossword::WordSetT temp_domain;
  for(auto& domain_in_x: domain_x){
     bool is_possible_value = false;
     auto& char_in_x = domain_in_x[_overlap.value().first];
     for(auto& domain_in_y: domain_y){  
      auto& char_in_y = domain_in_y[_overlap.value().second];
      if(char_in_x == char_in_y){
        temp_domain.emplace(domain_in_x);
        break;
      }
     }
  }
  bool isRevise = (temp_domain.size() != domain_x.size());
  domains[x] = temp_domain;
  return isRevise;
}

bool CrosswordSolver::ac3(const std::optional<CrosswordSolver::ArcSetT>& arcs_opt) noexcept
{
  if (!arcs_opt) {
    return true;
  }

  CrosswordSolver::ArcSetT _arcs = std::move(*arcs_opt); 
  while (!_arcs.empty())
  {
    auto [x, y] = _arcs.front();
    _arcs.pop();
    if(revise(x, y)){
      auto& _variable_domain = domains.at(x);
      if (_variable_domain.empty()) {
        return false;
      }
      const auto& x_neighbor = crossword.neighbors(*x);
      for(auto z: x_neighbor){
        if(z != y){
          _arcs.push(std::make_pair(z, x));
        }
      }
    }
  }

  return true;
}

bool CrosswordSolver::is_assignment_complete(const CrosswordSolver::AssignmentT& assignment) const noexcept
{

  for(auto& var: crossword.get_variables()){
    try
    {
      auto& current_assignment = assignment.at(&var);
    }
    catch(const std::exception& e)
    {
      return false;
    }
  }
  return true;
}

bool CrosswordSolver::is_assignment_consistent(const CrosswordSolver::AssignmentT& assignment) const noexcept
{
  // Verificar restricciones de longitud
  for (auto& [_var, _word] : assignment) {
      if (_word.length() != _var->get_cells().size()) {
          return false;
      }
  }

  // Verificar restricciones de valor único
  std::unordered_set<std::string> values;
  for (auto& [_var, _word] : assignment) {
      if (values.count(_word) > 0) {
          return false;
      }
      values.insert(_word);
  }

  // Verificar restricciones binarias
  for (auto& [_var, palabra] : assignment) {
    for (auto& neighbor : crossword.neighbors(*_var)) {
      if (assignment.count(neighbor) > 0) {
        auto _overlap = crossword.get_overlaps().at(std::make_pair(_var, neighbor));
        auto neighbor_value = assignment.at(neighbor);
        auto common_neighbor = neighbor_value[_overlap.value().second];
        if (palabra[_overlap.value().first] != common_neighbor) {
          return false;
        }
      }
    }
  }

  return true;
}

std::vector<std::string> CrosswordSolver::order_domain_values(const Variable* var, const AssignmentT& assignment) const noexcept
{
    auto& domain_values = domains.at(var);
    std::vector<std::pair<int, std::string>> value_scores; 
    
    for (const auto& value : domain_values) {
      int count = 0;
      for (const auto& _neighbor : crossword.neighbors(*var)) {
        auto& _overlap =  crossword.get_overlaps().at(std::make_pair(var, _neighbor));
        if (assignment.find(_neighbor) == assignment.end()) { 
          auto& _neighbor_domains = domains.at(_neighbor);
          for (const auto& neighbor_value : _neighbor_domains) {
            auto& _neighbor_overlap = crossword.get_overlaps().at(std::make_pair(_neighbor, var));
            if (
              value != neighbor_value && 
              _neighbor_overlap.value().first == _overlap.value().first &&
              _neighbor_overlap.value().second == _overlap.value().second
            ) {
              ++count; 
            }
          }
        }
      }
      value_scores.emplace_back(count, value);
    }
    
    std::sort(value_scores.begin(), value_scores.end());
    
    std::vector<std::string> ordered_values;
    for (const auto& score_value : value_scores) {
        ordered_values.push_back(score_value.second);
    }
    return ordered_values;
}

const Variable* CrosswordSolver::select_unassigned_variable(const AssignmentT& assignment) const noexcept
{
   // Creamos una lista de variables no asignadas
    std::vector<const Variable*> unassigned_variables;
    for (const auto& variable : crossword.get_variables()) {
        if (assignment.at(&variable).length() == 0) {
          unassigned_variables.push_back(&variable);
        }
    }

    // Si solo queda una variable no asignada, la devolvemos
    if (unassigned_variables.size() == 1) {
        return unassigned_variables[0];
    }

    // Ordenamos la lista de variables no asignadas por la heurística de valor mínimo restante
    std::sort(
      unassigned_variables.begin(), 
      unassigned_variables.end(), 
      [&](const Variable* a, const Variable* b) {
      return domains.at(a).size() < domains.at(b).size();
    });

    // Seleccionamos la variable con el menor número de valores restantes en su dominio
    const Variable* selected_variable = unassigned_variables[0];
    int min_domain_size = domains.at(selected_variable).size();

    // Buscamos si hay otras variables con el mismo número de valores restantes
    std::vector<const Variable*> same_domain_size_variables;
    for (size_t i = 1; i < unassigned_variables.size(); ++i) {
        const Variable* variable = unassigned_variables[i];
        if (domains.at(variable).size() == min_domain_size) {
          same_domain_size_variables.push_back(variable);
        } else {
          break;
        }
    }

    // Si solo hay una variable con el menor número de valores restantes, la devolvemos
    if (same_domain_size_variables.size() == 0) {
        return selected_variable;
    }

    // Si hay varias variables con el mismo número de valores restantes, las ordenamos por la heurística de grado
    std::sort(
      same_domain_size_variables.begin(), 
      same_domain_size_variables.end(),
      [&](const Variable* a, const Variable* b) {
        return crossword.neighbors(*a).size() > crossword.neighbors(*b).size();
    });

    // Devolvemos la variable con el mayor grado entre las variables con el mismo número de valores restantes
    return same_domain_size_variables[0];
}

std::optional<CrosswordSolver::AssignmentT> CrosswordSolver::backtrack(const AssignmentT& assignment) noexcept
{ 
  // if (is_assignment_complete(assignment)) {
  //   return assignment;
  // }

  // const Variable* var = select_unassigned_variable(assignment);

  // for (const auto& value : order_domain_values(var, assignment)) {
  //   AssignmentT updated_assignment = assignment;
  //   updated_assignment[var] = value;

  //   if (is_assignment_consistent(updated_assignment)) {
  //     std::optional<AssignmentT> result = backtrack(updated_assignment);
  //     // el pedazo
  //     if (result.has_value()) {
  //       return result;
  //     }
  //   }
  // }

  return std::nullopt;
}