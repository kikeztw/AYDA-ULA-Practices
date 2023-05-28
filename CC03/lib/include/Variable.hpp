
#pragma once

#include <optional>
#include <string>
#include <vector>

/**
 * @brief A crossword variable.
 * 
 * This class represents a variable with starting point, direction, and length.
 * 
 * It exports the following operators:
 *  - == A variable is equal to another one if and only if all of the values position, direction, and length are equal.
 *  - != A variable is different to another one if and only if one of the values position, direction, or length are different.
 *  - <  First it's considered the row component of the position, then it's considered the column component, finally, it's considered the length.
 * 
 * This class also exports the following structures to compute hashes and equal comparisons to be able to store them in hash tables.
 * 
 * struct Hash: Functor to compute the hash value for a simple Variable object.
 * struct PointerHash: Functor to compute the hash value for a pointer to a Variable object.
 * struct PointerPairHash: Functor to compute the hash value to a pair of pointers to Variable objects.
 * struct PointerEqual: Functor that compares equality between two pointers to Variable objects.
 * struct PointerEqual: Functor that compares equality between two pairs of pointers to Variable objects.
 */
class Variable
{
public:
  struct Hash
  {
    size_t operator () (const Variable& v) const noexcept;
  };

  struct PointerHash
  {
    size_t operator () (const Variable* v) const noexcept;
  };

  struct PointerPairHash
  {
    size_t operator () (const std::pair<const Variable*, const Variable*>& p) const noexcept;
  };

  struct PointerEqual
  {
    bool operator () (const Variable* v1, const Variable* v2) const noexcept;
  };

  struct PointerPairEqual
  {
    bool operator () (const std::pair<const Variable*, const Variable*>& p1, const std::pair<const Variable*, const Variable*>& p2) const noexcept;
  };

  enum class direction { across, down };

  Variable(size_t _i, size_t _j, direction _dir, size_t _length) noexcept;

  Variable(const Variable& v) noexcept;

  Variable(Variable&& v) noexcept;

  Variable& operator = (Variable v) noexcept;

  void swap(Variable& v) noexcept;

  std::pair<size_t, size_t> get_position() const noexcept;
  
  direction get_direction() const noexcept;

  const std::vector<std::pair<size_t, size_t>>& get_cells() const noexcept;

  bool operator == (const Variable& v) const noexcept;
  
  bool operator != (const Variable& v) const noexcept;

  bool operator < (const Variable& v) const noexcept;

  std::string to_string() const noexcept;

  /**
   * @brief Compute the intersection between this and v.
   * 
   * A intersection is defined as a pair of indexs (i, j), where i is the position of the common character
   * in this and j is the position of the common character in v.
   * 
   * If there is no intersection, then the result is nullopt.
   * 
   * @param v Another variable.
   * @return The intersection or nullopt if there is no intersection.
   */
  std::optional<std::pair<size_t, size_t>> intersection_with(const Variable& v) const noexcept;

private:
  size_t i;
  size_t j;
  direction dir;
  std::vector<std::pair<size_t, size_t>> cells;
};


