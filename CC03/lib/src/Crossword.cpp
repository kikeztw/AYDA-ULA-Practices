#include <algorithm>
#include <fstream>

#include <Crossword.hpp>

Crossword::Crossword(std::istream& in_words, std::istream& in_structure) noexcept
  : words{}, structure{}, variables{}, overlaps{}
{
  // Load the vocavulary set
  std::string w{};
  while (std::getline(in_words, w))
  {
    std::transform(w.begin(), w.end(), w.begin(), toupper);
    words.insert(w);
  }

  // Determine the structure of crossword
  std::vector<std::string> contents;
  std::string line{};
  while (std::getline(in_structure, line))
  {
    contents.push_back(line);
  }

  for (size_t i = 0; i < contents.size(); ++i)
  {
    structure.push_back(std::vector<bool>{});
    auto& row = structure.back();

    for (size_t j = 0; j < contents[i].size(); ++j)
    {
      if (j >= contents[i].size())
      {
        row.push_back(false);
      }
      else if (contents[i][j] == '_')
      {
        row.push_back(true);
      }
      else
      {
        row.push_back(false);
      }
    }
  }

  // Determine variable set
  for (size_t i = 0; i < structure.size(); ++i)
  {
    for (size_t j = 0; j < structure[i].size(); ++j)
    {
      // Vertical words
      bool start_word = structure[i][j] && (i == 0 || !structure[i - 1][j]);

      if (start_word)
      {
        size_t length = 1;
        for (size_t k = i + 1; k < structure.size() && structure[k][j]; ++k, ++length);

        if (length > 1)
        {
          variables.insert(Variable{i, j, Variable::direction::down, length});
        }
      }

      // Horizontal words
      start_word =  structure[i][j] && (j == 0 || !structure[i][j - 1]);

      if (start_word)
      {
        size_t length = 1;
        for (size_t l = j + 1; l < structure[i].size() && structure[i][l]; ++l, ++length);

        if (length > 1)
        {
          variables.insert(Variable{i, j, Variable::direction::across, length});
        }
      }
    }
  }

  /* Comṕute overlaps for each word.
    For any pair of variables (v1, v2), their overlap will be a pair (i, j) where i is the position of the
    character in v1 and j is the position of the character in v2. If not overlap, them the overlap will be a null option.
  */
  for (const Variable& v1: variables)
  {
    for (const Variable& v2: variables)
    {
      if (v1 == v2)
      {
        continue;
      }

      overlaps.emplace(std::make_pair(&v1, &v2), v1.intersection_with(v2));
    }
  }
}

Crossword::Crossword(const Crossword& cw) noexcept
  : words{cw.words}, structure{cw.structure}, variables{cw.variables}, overlaps{cw.overlaps}
{
  
}

Crossword::Crossword(Crossword&& cw) noexcept
  : words{}, structure{}, variables{}, overlaps{}
{
  swap(cw);
}

Crossword& Crossword::operator = (Crossword cw) noexcept
{
  swap(cw);
  return *this;
}

void Crossword::swap(Crossword& cw) noexcept
{
  std::swap(words, cw.words);
  std::swap(structure, cw.structure);
  std::swap(variables, cw.variables);
  std::swap(overlaps, cw.overlaps);
}

std::unordered_set<const Variable*> Crossword::neighbors(const Variable& var) const noexcept
{
  std::unordered_set<const Variable*> ns{};

  for (const Variable& v: variables)
  {
    if (v != var && overlaps.at(std::make_pair(&v, &var)) != std::nullopt)
    {
      ns.insert(&v);
    }
  }

  return ns;
}

const Crossword::WordSetT& Crossword::get_words() const noexcept
{
  return words;
}

const Crossword::StructureT& Crossword::get_structure() const noexcept
{
  return structure;
}

const Crossword::VariableSetT& Crossword::get_variables() const noexcept
{
  return variables;
}

const Crossword::OverlapSetT& Crossword::get_overlaps() const noexcept
{
  return overlaps;
}

size_t Crossword::width() const noexcept
{
  return structure[0].size();
}

size_t Crossword::height() const noexcept
{
  return structure.size();
}
