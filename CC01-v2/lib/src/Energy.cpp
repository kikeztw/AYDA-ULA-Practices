#include <cmath>
#include <iostream>


#include <Color.hpp>
#include <Energy.hpp>

using namespace std;


Energy::Energy(const std::vector<std::vector<Color>> &_pixels) noexcept
    : pixels(_pixels)
{
}

std::vector<std::vector<unsigned>> Energy::compute_energy() const noexcept
{
  // TODO
  long height = pixels.size();
  long width = pixels[0].size();
  std::vector<std::vector<unsigned>> matriz_ener(height, std::vector<unsigned>(width, 0));

for (long i = 0; i < height; i++){
  for (long j = 0; j < width; j++){
    matriz_ener[i][j] = energy_at(i,j);
  }
}

  return matriz_ener;
}

std::vector<std::vector<Color>> Energy::energy_to_colors(const std::vector<std::vector<unsigned>> &energy)
{
  std::vector<std::vector<Color>> colors{energy.size(), std::vector<Color>{}};

  unsigned max_energy = 0;

  for (size_t i = 0; i < energy.size(); ++i)
  {
    for (unsigned e : energy[i])
    {
      if (e > max_energy)
      {
        max_energy = e;
      }
    }
  }

  for (size_t i = 0; i < energy.size(); ++i)
  {
    for (unsigned e : energy[i])
    {
      // Normalize the energy value to be in [0, 255]
      uint8_t energy_normalized = round(255.f * float(e) / float(max_energy));
      // Convert the value into a grayscale color.
      colors[i].push_back(Color{energy_normalized, energy_normalized, energy_normalized});
    }
  }
  return colors;
}

unsigned Energy::energy_at(size_t i, size_t j) const noexcept
{
  // TODO
  int rx,ry, gx, gy, bx, by;
  long height = pixels.size();
  long width = pixels[0].size();

/*  switch (j)
  {
  case 0:
    rx = pixels[i][j].get_red() - pixels[i][j + 1].get_red();
    gx = pixels[i][j].get_green() - pixels[i][j + 1].get_green();
    bx = pixels[i][j].get_blue() - pixels[i][j + 1].get_blue();
    break;
  case j-1:
    rx = pixels[i][j].get_red() - pixels[i][j - 1].get_red();
    gx = pixels[i][j].get_green() - pixels[i][j - 1].get_green();
    bx = pixels[i][j].get_blue() - pixels[i][j - 1].get_blue();
    break;
  default:
    rx = pixels[i][j - 1].get_red() - pixels[i][j + 1].get_red();
    gx = pixels[i][j - 1].get_green() - pixels[i][j + 1].get_green();
    bx = pixels[i][j - 1].get_blue() - pixels[i][j + 1].get_blue();
    break;
  }*/


  if (j == 0) 
  {
    rx = pixels[i][j].get_red() - pixels[i][j + 1].get_red();
    gx = pixels[i][j].get_green() - pixels[i][j + 1].get_green();
    bx = pixels[i][j].get_blue() - pixels[i][j + 1].get_blue();
  }
  else if (j == pixels[0].size() - 1) 
  {
    rx = pixels[i][j].get_red() - pixels[i][j - 1].get_red();
    gx = pixels[i][j].get_green() - pixels[i][j - 1].get_green();
    bx = pixels[i][j].get_blue() - pixels[i][j - 1].get_blue();
  }
  else 
  {
    rx = pixels[i][j - 1].get_red() - pixels[i][j + 1].get_red();
    gx = pixels[i][j - 1].get_green() - pixels[i][j + 1].get_green();
    bx = pixels[i][j - 1].get_blue() - pixels[i][j + 1].get_blue();
  }

  // Calculo verticalmente
  if (i == 0) 
  {
    ry = pixels[i][j].get_red() - pixels[i + 1][j].get_red();
    gy = pixels[i][j].get_green() - pixels[i + 1][j].get_green();
    by = pixels[i][j].get_blue() - pixels[i + 1][j].get_blue();
  }
  else if (i == pixels.size() - 1) 
  {
    ry = pixels[i][j].get_red() - pixels[i - 1][j].get_red();
    gy = pixels[i][j].get_green() - pixels[i - 1][j].get_green();
    by = pixels[i][j].get_blue() - pixels[i - 1][j].get_blue();
  }
  else 
  {
    ry = pixels[i - 1][j].get_red() - pixels[i + 1][j].get_red();
    gy = pixels[i - 1][j].get_green() - pixels[i + 1][j].get_green();
    by = pixels[i - 1][j].get_blue() - pixels[i + 1][j].get_blue();
  }

// devuelvo el valor de la ecuacion
  return (pow(rx, 2) + pow(gx, 2) + pow(bx, 2) + pow(ry, 2) + pow(gy, 2) + pow(by, 2));
}
