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
  cout <<"<------------Printf Cumulative Matrix--------------->"<< endl;

  std::vector<std::vector<std::pair<std::size_t, long>>> cumulative = getCumulativeEnergyMatrix(energy);

  for (size_t i = 0; i < cumulative.size(); i++)
  {
    for (size_t j = 0; j < cumulative[0].size(); j++)
    {
      std::cout<<cumulative[i][j].second<<" - ";
    }
    std::cout<<endl;
  }

  cout <<"<---- Results compute_horizontal_seam for test number: ("<<test_number<<") ---->"<< endl;
  Seam seam(energy);
  auto computed_seam = seam.compute_horizontal_seam();
  cout << "Min Value index :"<<computed_seam.first<< endl;
  cout << "Min Value: "<<computed_seam.second<< endl;
  cout << "End test number: "<<test_number<<endl;
}

int main(int argc, char* argv[])
{
  

  std::vector<std::vector<unsigned>> energy_1 = {
    {18630, 23335, 63387, 87042, 84798, 112898, 65710},
    {27029, 72122, 16974, 41136, 95424, 46316, 83194},
    {17428, 29814, 73442, 71691, 44793, 144686, 19045},
    {97650, 22762, 16046, 4562, 33790, 92039, 67119},
    {108324, 83947, 77598, 60218, 71861, 82420, 8695},
    {54175, 80939, 84567, 147490, 52392, 107500, 66171},
    {80263, 35460, 115603, 22656, 135277, 100460, 21242},
    {105086, 97651, 80978, 73115, 59778, 19255, 102585},
    {85739, 20473, 82839, 26798, 88205, 31737, 97264},
    {52156, 76298, 24622, 52010, 10941, 58913, 20973},
    {41332, 71688, 37121, 34278, 27954, 84578, 94118},
    {66887, 31205, 39548, 27594, 76677, 37383, 123311},
  };

  std::vector<std::vector<unsigned>> energy_2 = {
    {51664, 20040, 64789, 65123},
    {23944, 120269, 38129, 74867},
    {118521, 36226, 98562, 77895},
    {62650, 101312, 110204, 96412},
    {38539, 77963, 80450, 83259},
    {55136, 98450, 66830, 38560},
    {82238, 41250, 50246, 16026},
  };


  testRun(energy_1, 1);
  testRun(energy_2, 2);

  return EXIT_SUCCESS;
}
