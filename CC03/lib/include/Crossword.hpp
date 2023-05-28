#pragma once

#include <istream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <Variable.hpp>

class Crossword
{
public:
  /// Alias for a words set. A word set stores the words universe to solve the crossword.
  using WordSetT = std::unordered_set<std::string>;

  /// Alias for a crossword structure. It is defined by a matrix of booleans, where a true value represents a valid cell to write a value.
  using StructureT = std::vector<std::vector<bool>>;

  /// Alias for a variable set.
  using VariableSetT = std::unordered_set<Variable, Variable::Hash>;

  /// Alias for an overlaps set. This relates each pair of variables with the intersection between them.
  using OverlapSetT = std::unordered_map<std::pair<const Variable*, const Variable*>, std::optional<std::pair<size_t, size_t>>, Variable::PointerPairHash, Variable::PointerPairEqual>;

  Crossword(std::istream& in_words, std::istream& in_structure) noexcept;

  Crossword(const Crossword& cw) noexcept;

  Crossword(Crossword&& cw) noexcept;

  Crossword& operator = (Crossword cw) noexcept;

  void swap(Crossword& cw) noexcept;

  std::unordered_set<const Variable*> neighbors(const Variable& var) const noexcept;

  const WordSetT& get_words() const noexcept;

  const StructureT& get_structure() const noexcept;

  const VariableSetT& get_variables() const noexcept;

  const OverlapSetT& get_overlaps() const noexcept;

  size_t width() const noexcept;

  size_t height() const noexcept;

private:
  WordSetT words;
  StructureT structure;
  VariableSetT variables;
  OverlapSetT overlaps;
};
