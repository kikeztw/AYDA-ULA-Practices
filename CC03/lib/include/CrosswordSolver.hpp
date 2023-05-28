#pragma once

#include <queue>

#include <Crossword.hpp>

class CrosswordSolver
{
public:
  using AssignmentT = std::unordered_map<const Variable*, std::string, Variable::PointerHash, Variable::PointerEqual>;
  using ArcSetT = std::queue<std::pair<const Variable*, const Variable*>>;

  CrosswordSolver(const Crossword& _crossword) noexcept;

  void print(const AssignmentT& assignment, std::ostream& out) const noexcept;

  std::vector<std::vector<char>> letter_grid(const AssignmentT& assignment) const noexcept;

  std::optional<AssignmentT> solve() noexcept;

  void enforce_node_consistency() noexcept;

  bool revise(const Variable* x, const Variable* y) noexcept;

  bool ac3(const std::optional<ArcSetT>& arcs_opt = std::nullopt) noexcept;

  bool is_assignment_complete(const AssignmentT& assignment) const noexcept;

  bool is_assignment_consistent(const AssignmentT& assignment) const noexcept;

  std::vector<std::string> order_domain_values(const Variable* var, const AssignmentT& assignment) const noexcept;

  const Variable* select_unassigned_variable(const AssignmentT& assignment) const noexcept;

  std::optional<AssignmentT> backtrack(const AssignmentT& assignment) noexcept;

private:
  const Crossword& crossword;
  std::unordered_map<const Variable*, Crossword::WordSetT, Variable::PointerHash, Variable::PointerEqual> domains;
};
