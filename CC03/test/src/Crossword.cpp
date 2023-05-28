#include <sstream>

#include <Crossword.hpp>
#include <TestUtils.hpp>

using namespace std;

struct Data
{
  static const string structure;
  static const string words;
  
  static const Crossword::WordSetT expected_words;
  static const Crossword::StructureT expected_structure;
  static const Crossword::VariableSetT expected_variables;
  static const Crossword::OverlapSetT expected_overlaps;

  static const Variable* lookup_var(size_t i, size_t j, Variable::direction d, size_t l)
  {
    return &*expected_variables.find(Variable{i, j, d, l});
  }
};

const string Data::structure{"#___#\n#_##_\n#_##_\n#_##_\n#____"};
const string Data::words{"one\ntwo\nthree\nfour\nfive\nsix\nseven\neight\nnine\nten"};

const Crossword::WordSetT Data::expected_words
{
  "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN"
};

const Crossword::StructureT Data::expected_structure
{
  {false, true, true, true, false},
  {false, true, false, false, true},
  {false, true, false, false, true},
  {false, true, false, false, true},
  {false, true, true, true, true}
};

const Crossword::VariableSetT Data::expected_variables
{
  Variable{0, 1, Variable::direction::down, 5},
  Variable{0, 1, Variable::direction::across, 3},
  Variable{4, 1, Variable::direction::across, 4},
  Variable{1, 4, Variable::direction::down, 4}
};

const Crossword::OverlapSetT Data::expected_overlaps
{
  make_pair(make_pair(Data::lookup_var(0, 1, Variable::direction::down, 5), Data::lookup_var(0, 1, Variable::direction::across, 3)), make_pair(0, 0)),
  make_pair(make_pair(Data::lookup_var(0, 1, Variable::direction::across, 3), Data::lookup_var(0, 1, Variable::direction::down, 5)), make_pair(0, 0)),
  make_pair(make_pair(Data::lookup_var(0, 1, Variable::direction::down, 5), Data::lookup_var(4, 1, Variable::direction::across, 4)), make_pair(4, 0)),
  make_pair(make_pair(Data::lookup_var(4, 1, Variable::direction::across, 4), Data::lookup_var(0, 1, Variable::direction::down, 5)), make_pair(0, 4)),
  make_pair(make_pair(Data::lookup_var(0, 1, Variable::direction::down, 5), Data::lookup_var(1, 4, Variable::direction::down, 4)), nullopt),
  make_pair(make_pair(Data::lookup_var(1, 4, Variable::direction::down, 4), Data::lookup_var(0, 1, Variable::direction::down, 5)), nullopt),
  make_pair(make_pair(Data::lookup_var(0, 1, Variable::direction::across, 3), Data::lookup_var(4, 1, Variable::direction::across, 4)), nullopt),
  make_pair(make_pair(Data::lookup_var(4, 1, Variable::direction::across, 4), Data::lookup_var(0, 1, Variable::direction::across, 3)), nullopt),
  make_pair(make_pair(Data::lookup_var(0, 1, Variable::direction::across, 3), Data::lookup_var(1, 4, Variable::direction::down, 4)), nullopt),
  make_pair(make_pair(Data::lookup_var(1, 4, Variable::direction::down, 4), Data::lookup_var(0, 1, Variable::direction::across, 3)), nullopt),
  make_pair(make_pair(Data::lookup_var(4, 1, Variable::direction::across, 4), Data::lookup_var(1, 4, Variable::direction::down, 4)), make_pair(3, 3)),
  make_pair(make_pair(Data::lookup_var(1, 4, Variable::direction::down, 4), Data::lookup_var(4, 1, Variable::direction::across, 4)), make_pair(3, 3))
};

void test_crossword()
{
  cout << "Given the structure\n";
  cout << Data::structure << endl;
  cout << "And the word list\n";
  cout << Data::words << endl;

  stringstream s_structure;
  s_structure << Data::structure;
  
  stringstream s_words;
  s_words << Data::words;

  Crossword cw{s_words, s_structure};

  cout << "When the method get_words is called, then it should return " << Data::expected_words << endl;
  assert_equal(cw.get_words(), Data::expected_words);

  cout << "When the method get_structure is called, then it should return\n" << Data::expected_structure << endl;
  assert_equal(cw.get_structure(), Data::expected_structure);

  cout << "When the method get_variables is called, then it should return " << Data::expected_variables << endl;
  assert_equal(cw.get_variables(), Data::expected_variables);

  cout << "When the method get_overlaps is called, then it should return " << Data::expected_overlaps << endl;
  assert_equal(cw.get_overlaps(), Data::expected_overlaps);
}

int main()
{
  cout << "Testing Crossword\n";
  test_crossword();
  cout << "Everything ok!\n";
  return EXIT_SUCCESS;
}