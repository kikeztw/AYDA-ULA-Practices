#include <iostream>

#include <Color.hpp>
#include <Energy.hpp>
#include <Seam.hpp>
#include <Png.hpp>

using namespace std;


void testRun (std::vector<std::vector<unsigned>> energy, int test_number)
{
  cout <<endl;
  cout <<endl;
  cout <<"<------------"<<"Running Test Number ("<<test_number<<") --------------->"<<endl;
  cout <<endl;
  long row_size = energy.size();
  long column_size = energy[0].size();

  cout <<"<---- Results compute_horizontal_seams for test number: ("<<test_number<<") ---->"<< endl;

  Seam seam(energy);
  auto computed_seam = seam.compute_horizontal_seams();

  std::cout<<"Current j-coordinates list: { ";
  for (size_t i = 0; i < computed_seam.first.size(); i++)
  {
    std::cout<<computed_seam.first[i]<<",";
  }
  std::cout<<" }";
  cout <<endl;
  std::cout<<"Current lowest energy: "<<computed_seam.second<<endl;
  cout <<endl;
  cout <<endl;
}


int main(int argc, char* argv[])
{
 
  std::vector<std::vector<unsigned>> energy_1 = {
    {30610, 64514, 53709, 47174, 61106, 69287, 36430},
    {38620, 76399, 38522, 76890, 44587, 84311, 3530},
    {104799, 106286, 89023, 75518, 53108, 68502, 49151},
    {44964, 45243, 6580, 74720, 53092, 70339, 74364},
    {64391, 90070, 72472, 49086, 54590, 30726, 24205},
    {29819, 82913, 90239, 46912, 18870, 67169, 61203},
    {70584, 38035, 87739, 10053, 59619, 31704, 79786},
    {66273, 57990, 18140, 16639, 46991, 21579, 49704},
    {49252, 29932, 65067, 17771, 71296, 44136, 71899},
    {36053, 54780, 94039, 54153, 45560, 138824, 58690},
    {37287, 114747, 79748, 27022, 57209, 97118, 11401},
    {45035, 37371, 90713, 44283, 80529, 81175, 129471},
  };

  std::vector<std::vector<unsigned>> energy_2 = {
    {54618, 62651, 92445, 82950, 15949, 73354, 131526},
    {96458, 56512, 18607, 32439, 119587, 49406, 93575},
    {82111, 21064, 31935, 70317, 76390, 64322, 20198},
    {54247, 25232, 69777, 53560, 37049, 44328, 35396},
    {13403, 70319, 81378, 7668, 79972, 48348, 78322},
    {41659, 45259, 98525, 61023, 28064, 71940, 29703},
    {107551, 45053, 30309, 66336, 81192, 103580, 124016},
    {25340, 107209, 64916, 91395, 28275, 127668, 94594},
    {87182, 67095, 51435, 56861, 134273, 51478, 153352},
    {14415, 87087, 59755, 41089, 73027, 100585, 137918},
    {90498, 59859, 113302, 51943, 75996, 78907, 90263},
    {121320, 90323, 35876, 46167, 127703, 108872, 68033},
  };


  testRun(energy_1, 1);
  testRun(energy_2, 2);


  return EXIT_SUCCESS;
}
