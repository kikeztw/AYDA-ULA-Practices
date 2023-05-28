#include <TestUtils.hpp>

#include <Variable.hpp>

using namespace std;

struct Data
{
  static constexpr size_t i{12};
  static constexpr size_t j{4};
  static constexpr size_t length{6};

  static const vector<pair<size_t, size_t>> expected_across_cells;
  static const vector<pair<size_t, size_t>> expected_down_cells;

  static const Variable v1;
  static const Variable v2;
  static const Variable v3;

  static const optional<pair<size_t, size_t>> expected_v1v2_intersection;
  static const optional<pair<size_t, size_t>> expected_v1v3_intersection;
};

const vector<pair<size_t, size_t>> Data::expected_across_cells
{
  make_pair(12, 4), make_pair(12, 5), make_pair(12, 6), make_pair(12, 7), make_pair(12, 8), make_pair(12, 9)
};

const vector<pair<size_t, size_t>> Data::expected_down_cells
{
  make_pair(12, 4), make_pair(13, 4), make_pair(14, 4), make_pair(15, 4), make_pair(16, 4), make_pair(17, 4),
};

const Variable Data::v1{3, 4, Variable::direction::across, 7};
const Variable Data::v2{2, 6, Variable::direction::down, 5};
const Variable Data::v3{5, 7, Variable::direction::down, 6};

const optional<pair<size_t, size_t>> Data::expected_v1v2_intersection{make_pair(2, 1)};
const optional<pair<size_t, size_t>> Data::expected_v1v3_intersection{nullopt};

void test_across_variable()
{
  cout << "Given the position (" << Data::i << ", " << Data::j << ") and the length " << Data::length << endl;
  cout << "When a variable is created with direction across, then it should contain the following cells " << Data::expected_across_cells << endl;
  Variable v{Data::i, Data::j, Variable::direction::across, Data::length};
  assert_equal(v.get_cells(), Data::expected_across_cells);
}

void test_down_variable()
{
    cout << "Given the position (" << Data::i << ", " << Data::j << ") and the length " << Data::length << endl;
    cout << "When a variable is created with direction down, then it should contain the following cells " << Data::expected_down_cells << endl;
    Variable v{Data::i, Data::j, Variable::direction::down, Data::length};
    assert_equal(v.get_cells(), Data::expected_down_cells);
}

void test_intersection()
{
  cout << "Given the variables " << Data::v1 << " and " << Data::v2 << endl;
  cout << "When the method intersection_with is called, then it should return " << Data::expected_v1v2_intersection.value() << endl;
  auto result = Data::v1.intersection_with(Data::v2);
  assert_equal(result, Data::expected_v1v2_intersection);
}

void test_no_intersection()
{
  cout << "Given the variables " << Data::v1 << " and " << Data::v3 << endl;
  cout << "When the method intersection_with is called, then it should return nullopt" << endl;
  auto result = Data::v1.intersection_with(Data::v3);
  assert_equal(result, Data::expected_v1v3_intersection);
}

int main()
{
  cout << "Testing Variable\n";

  test_across_variable();
  test_down_variable();
  test_intersection();
  test_no_intersection();

  cout << "Everything ok!\n";
  return EXIT_SUCCESS;
}
