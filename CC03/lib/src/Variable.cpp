#include <sstream>

#include <HashHelpers.hpp>
#include <Variable.hpp>

size_t Variable::Hash::operator () (const Variable& v) const noexcept
{
  return hash_val(v.i, v.j, static_cast<int>(v.dir), v.cells.size());
}

size_t Variable::PointerHash::operator ()(const Variable* v) const noexcept
{
  return hash_val(v->i, v->j, static_cast<int>(v->dir), v->cells.size());
}

size_t Variable::PointerPairHash::operator ()(const std::pair<const Variable*, const Variable*>& p) const noexcept
{
  return hash_val(p.first->i, p.first->j, static_cast<int>(p.first->dir), p.first->cells.size(),
                  p.second->i, p.second->j, static_cast<int>(p.second->dir), p.second->cells.size());
}

bool Variable::PointerEqual::operator () (const Variable* v1, const Variable* v2) const noexcept
{
  return *v1 == *v2;
}

bool Variable::PointerPairEqual::operator () (const std::pair<const Variable*, const Variable*>& p1, const std::pair<const Variable*, const Variable*>& p2) const noexcept
{
  return *p1.first == *p2.first && *p1.second == *p2.second;
}

Variable::Variable(size_t _i, size_t _j, direction _dir, size_t _length) noexcept
  : i{_i}, j{_j}, dir{_dir}, cells{_length}
{
  size_t h_factor = dir == direction::across ? 1 : 0;
  size_t v_factor = dir == direction::across ? 0 : 1;

  for (size_t k = 0; k < _length; ++k)
  {
    cells[k] = std::make_pair(i + k * v_factor, j + k * h_factor);
  }
}

Variable::Variable(const Variable& v) noexcept
  : i{v.i}, j{v.j}, dir{v.dir}, cells{v.cells}
{
  
}

Variable::Variable(Variable&& v) noexcept
  : i{}, j{}, dir{}, cells{}
{
  swap(v);
}

Variable& Variable::operator = (Variable v) noexcept
{
  swap(v);
  return *this;
}

void Variable::swap(Variable& v) noexcept
{
  std::swap(i, v.i);
  std::swap(j, v.j);
  std::swap(dir, v.dir);
  std::swap(cells, v.cells);
}

std::pair<size_t, size_t> Variable::get_position() const noexcept
{
  return std::make_pair(i, j);
}

Variable::direction Variable::get_direction() const noexcept
{
  return dir;
}

const std::vector<std::pair<size_t, size_t>>& Variable::get_cells() const noexcept
{
  return cells;
}

bool Variable::operator == (const Variable& v) const noexcept
{
  return i == v.i && j == v.j && dir == v.dir && cells.size() == v.cells.size();
}

bool Variable::operator != (const Variable& v) const noexcept
{
  return i != v.i || j != v.j || dir != v.dir || cells.size() != v.cells.size();
}

bool Variable::operator < (const Variable& v) const noexcept
{
  if (i < v.i)
  {
    return true;
  }

  if (i > v.i)
  {
    return false;
  }

  if (j < v.j)
  {
    return true;
  }

  if (j > v.j)
  {
    return false;
  }

  return cells.size() < v.cells.size();
}

std::string Variable::to_string() const noexcept
{
  std::stringstream s;
  s << "Variable(" << i << ", " << j << ", " << (dir == direction::across ? "across" : "down") << ", " << cells.size() << ")";
  return s.str();
}

std::optional<std::pair<size_t, size_t>> Variable::intersection_with(const Variable& v) const noexcept
{
  for (size_t i = 0; i < cells.size(); ++i)
  {
    for (size_t j = 0; j < v.cells.size(); ++j)
    {
      if (cells[i] == v.cells[j])
      {
        return std::make_pair(i, j);
      }
    }
  }

  return std::nullopt;
}