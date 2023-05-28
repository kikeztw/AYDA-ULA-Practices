#include <fstream>
#include <iostream>

#include <Crossword.hpp>
#include <CrosswordSolver.hpp>
#include <IOUtils.hpp>

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    cout << "Usage: " << argv[0] << " structure words\n";
    return EXIT_FAILURE;
  }

  ifstream in_structure{argv[1]};

  if (!in_structure)
  {
    cout << argv[1] << " not found\n";
    return EXIT_FAILURE;
  }

  ifstream in_words{argv[2]};

  if (!in_words)
  {
    cout << argv[2] << " not found\n";
    in_structure.close();
    return EXIT_FAILURE;
  }

  Crossword crossword{in_words, in_structure};
  in_structure.close();
  in_words.close();
  CrosswordSolver solver(crossword);
  auto result = solver.solve();

  if (!result.has_value())
  {
    cout << "No solution\n";
  }
  else
  {
    solver.print(result.value(), cout);
    cout << endl;
  }
  
  return EXIT_SUCCESS;
}
