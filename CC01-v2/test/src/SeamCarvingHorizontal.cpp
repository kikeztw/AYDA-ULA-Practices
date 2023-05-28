#include <iostream>
#include <string>

#include <Carve.hpp>
#include <Color.hpp>
#include <Png.hpp>

using namespace std;


int main(int argc, char* argv[])
{

  if (argc < 3)
  {
    cout << "Usage: " << argv[0] << " input num_seams_to_remove\n";
    return EXIT_FAILURE;
  }

  auto n = std::stoul(argv[2]);
  
  std::vector<vector<Color>> pixels = {
    {Color(41,209,83), Color(118,237,91), Color(39,243,193), Color(22,42,87), Color(75,143,132), Color(147,114,156), Color(112,174,222), Color(0,98,178), Color(241,92,104), Color(59,13,68)},
    {Color(150,128,146), Color(59,91,159), Color(213,122,98), Color(217,114,45), Color(239,110,69), Color(237,27,231), Color(107,71,125), Color(77,34,181), Color(128,244,114), Color(46,209,148)},
    {Color(45,133,138), Color(36,101,242), Color(42,68,179), Color(60,153,201), Color(170,227,87), Color(226,4,189), Color(242,2,193), Color(237,93,226), Color(188,147,140), Color(29,242,144)},
    {Color(250,39,179), Color(141,240,214), Color(177,180,231), Color(139,189,212), Color(74,79,122), Color(7,98,196), Color(1,235,246), Color(60,218,85), Color(242,216,123), Color(197,189,236)},
    {Color(22,233,139), Color(186,124,141), Color(221,57,178), Color(112,85,241), Color(191,22,122), Color(69,190,128), Color(254,214,189), Color(132,115,73), Color(12,162,27), Color(83,73,213)},
    {Color(18,254,145), Color(81,168,117), Color(89,30,70), Color(61,112,11), Color(30,56,118), Color(143,163,247), Color(185,23,111), Color(128,133,177), Color(153,209,99), Color(145,4,253)},
    {Color(234,170,102), Color(109,178,40), Color(172,203,216), Color(125,186,132), Color(62,17,218), Color(242,1,60), Color(206,70,169), Color(237,117,29), Color(250,46,15), Color(70,44,35)},
    {Color(146,187,248), Color(44,129,199), Color(211,142,15), Color(0,5,110), Color(102,68,195), Color(157,232,42), Color(99,84,80), Color(167,241,238), Color(209,133,40), Color(185,250,27)},
    {Color(248,68,63), Color(189,153,182), Color(168,209,18), Color(134,19,196), Color(0,3,88), Color(251,176,63), Color(91,184,114), Color(51,72,196), Color(120,149,41), Color(99,105,137)},
    {Color(207,9,211), Color(136,136,174), Color(120,91,48), Color(45,110,76), Color(36,124,205), Color(110,178,42), Color(253,192,144), Color(219,141,164), Color(119,246,57), Color(143,145,134)},
    {Color(179,23,57), Color(60,215,192), Color(8,216,147), Color(178,8,2), Color(38,111,130), Color(123,86,88), Color(221,8,105), Color(228,66,31), Color(127,128,133), Color(62,111,110)},
    {Color(66,133,83), Color(163,141,42), Color(239,182,67), Color(47,104,45), Color(203,254,79), Color(90,181,113), Color(241,106,19), Color(211,107,158), Color(238,11,7), Color(213,140,53)},
    {Color(9,83,128), Color(126,153,140), Color(24,54,12), Color(37,170,67), Color(143,193,183), Color(141,235,240), Color(97,122,120), Color(13,143,228), Color(97,18,134), Color(234,63,119)},
    {Color(63,72,42), Color(226,107,250), Color(195,94,200), Color(16,87,118), Color(188,96,34), Color(22,205,177), Color(248,207,140), Color(198,233,149), Color(235,207,191), Color(177,218,106)},
    {Color(162,147,226), Color(98,243,249), Color(77,35,169), Color(211,130,86), Color(102,20,91), Color(163,16,89), Color(32,185,152), Color(213,75,237), Color(17,226,191), Color(64,66,106)},
    {Color(82,114,130), Color(44,204,195), Color(196,108,36), Color(177,64,175), Color(232,26,83), Color(186,129,182), Color(96,82,233), Color(199,246,22), Color(173,99,34), Color(165,227,130)},
    {Color(121,75,206), Color(101,250,238), Color(217,40,228), Color(171,72,59), Color(59,170,172), Color(68,185,230), Color(193,132,194), Color(25,180,117), Color(98,16,84), Color(61,77,90)},
    {Color(73,139,54), Color(240,151,96), Color(102,200,61), Color(148,0,174), Color(146,244,96), Color(174,132,5), Color(82,177,15), Color(74,50,150), Color(108,248,131), Color(177,195,148)},
    {Color(106,100,60), Color(26,33,217), Color(242,193,213), Color(3,205,143), Color(228,211,144), Color(44,28,74), Color(231,101,219), Color(204,212,71), Color(148,147,179), Color(61,98,192)},
  };

  Carve carve{pixels};

  std::cout<<std::endl;
  std::cout<<std::endl;
  cout << "Removing the " << n << " lowest seams\n";
  auto resized_pixels = carve.remove_n_lowest_seams(n);

  size_t row_size = resized_pixels.size();
  size_t column_size = resized_pixels[0].size();

  for (size_t i = 0; i < row_size; i++)
  {
    for (size_t j = 0; j < column_size; j++)
    {
      std::cout<<resized_pixels[i][j].to_string()<<", ";
    }
  }
  
  std::cout<<"Original Image size: "<<std::endl;
  std::cout<<"width: "<<pixels.size()<<std::endl;
  std::cout<<"height: "<<pixels[0].size()<std::endl;
  std::cout<<"--------------------- "<<std::endl;
  std::cout<<"New Image size: "<<std::endl;
  std::cout<<"width: "<<row_size<<std::endl;
  std::cout<<"height: "<<column_size<<std::endl;
  std::cout<<"--------------------- "<<std::endl;

  std::cout<<std::endl;
  std::cout<<std::endl;

  cout << "Everything ok!" << endl;
  return EXIT_SUCCESS;
}
