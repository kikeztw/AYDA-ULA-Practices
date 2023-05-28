#include <Color.hpp>
#include <Seam.hpp>
#include <TestUtils.hpp>
#include <iostream>


Seam::Seam(const std::vector<std::vector<unsigned>> &_energy) noexcept
    : energy(_energy)
{
}

std::pair<size_t, unsigned> Seam::compute_horizontal_seam() const noexcept
{
  std::vector<std::vector<std::pair<std::size_t, long>>> cumulative_energy = getCumulativeEnergyMatrix(energy);
  //return std::make_pair(0, 0);
  return getMinEnergyInPixel(cumulative_energy);
}

std::pair<std::vector<size_t>, unsigned> Seam::compute_horizontal_seams() const noexcept
{
  std::vector<std::vector<std::pair<std::size_t, long>>> cumulative_energy = getCumulativeEnergyMatrix(energy);
  return optimalPath(cumulative_energy);
}

std::vector<std::vector<Color>> Seam::min_seam_to_colors(const std::vector<std::vector<Color>>& pixels, size_t end_i) noexcept
{
  long h = pixels.size();
  long w = pixels[0].size();
  
  std::vector<std::vector<Color>> new_pixels{pixels};

  size_t min_i = std::max(long(end_i) - 5, 0L);
  size_t max_i = std::min(long(end_i) + 5, h - 1);

  size_t min_j = std::max(w - 11, 0L);
  
  for (size_t j = min_j; j < w; ++j)
  {
    for (size_t i = min_i; i <= max_i; ++i)
    {
      new_pixels[i][j] = Color(255, 0, 0);
    }
  }

  return new_pixels;
}

std::vector<std::vector<Color>> Seam::min_seams_to_colors(const std::vector<std::vector<Color>>& pixels, const std::vector<size_t>& seam_rows) noexcept
{
  long h = pixels.size();
  long w = pixels[0].size();
  
  std::vector<std::vector<Color>> new_pixels{pixels};

  for (size_t j = 0; j < seam_rows.size(); ++j)
  {

    size_t min_i = std::max(long(seam_rows[j]) - 2, 0L);
    size_t max_i = std::min(long(seam_rows[j]) + 2, h - 1);

    for (size_t i = min_i; i <= max_i; ++i)
    {
      new_pixels[i][j] = Color(255, 0, 0);
    }
  }

  return new_pixels;
}


std::pair<std::size_t, long> minBetweenTwoNeighbors(std::pair<std::size_t, long> a, std::pair<std::size_t, long> b) noexcept
{
  if(a.second < b.second){
    return a;
  }
  return b;
}

std::pair<std::size_t, long> minBetweenThreeNeighbors(std::pair<std::size_t, long> a, std::pair<std::size_t, long> b, std::pair<std::size_t, long> c = std::make_pair(0, 0)) noexcept
{
  if(a.second < b.second && a.second < c.second){
    return a;
  }
  if(b.second < a.second && b.second < c.second){
    return b;
  }
  
  return c;
}

std::vector<std::vector<std::pair<std::size_t, long>>> getCumulativeEnergyMatrix(const std::vector<std::vector<unsigned>> &energy) noexcept
{

  long row_size = energy.size();
  long column_size = energy[0].size();
 
  std::vector<std::vector<std::pair<std::size_t, long>>> accumulatedEnergy(row_size, std::vector<std::pair<std::size_t, long>>(column_size, std::make_pair(-1, -1)));

  for (int column = 0; column < column_size; column++)
  {
    for (int row = 0; row < row_size; row++)
    {
      unsigned energyInPixel = energy[row][column]; 

      if (column == 0){
        accumulatedEnergy[row][column] = std::make_pair(-1, energyInPixel);
        continue;
      }
      else if (row == 0 && column < 2)
      {
        std::pair<int, long> min = minBetweenTwoNeighbors(std::make_pair(row, energy[0][column - 1]), std::make_pair(1, energy[1][column - 1]));
        unsigned m = energyInPixel + min.second;
        accumulatedEnergy[row][column] = std::make_pair(min.first, m);
      }
      else if(row == 0 && column > 1){
        std::pair<int, long> min = minBetweenTwoNeighbors(std::make_pair(row, accumulatedEnergy[0][column - 1].second), std::make_pair(1, accumulatedEnergy[1][column - 1].second));
        unsigned m = energyInPixel + min.second;
        accumulatedEnergy[row][column] = std::make_pair(min.first, m);
      }
      else if (row == row_size - 1 && column < 2)
      {
        std::pair<int, long> min = minBetweenTwoNeighbors(std::make_pair(row - 1, energy[row - 1][column - 1]), std::make_pair(1, energy[row][column - 1]));
        unsigned m = energyInPixel + min.second;
        accumulatedEnergy[row][column] = std::make_pair(min.first, m);
      }
      else if (row == row_size - 1 && column > 1)
      {
        std::pair<int, long> min = minBetweenTwoNeighbors(std::make_pair(row - 1, accumulatedEnergy[row - 1][column - 1].second), std::make_pair(1, accumulatedEnergy[row][column - 1].second));
        unsigned m = energyInPixel + min.second;
        accumulatedEnergy[row][column] = std::make_pair(min.first, m);
      }
      else if(row != row_size - 1 && row != 0 && column < 2)
      {
        std::pair<int, long> min = minBetweenThreeNeighbors(std::make_pair(row - 1, energy[row - 1][column - 1]), std::make_pair(row, energy[row][column - 1]), std::make_pair(row + 1, energy[row + 1][column - 1]));
        unsigned m = energyInPixel + min.second;
        accumulatedEnergy[row][column] = std::make_pair(min.first, m);
      }
      else if (row != 0 && row != row_size - 1  && column > 1){
        std::pair<int, long> min = minBetweenThreeNeighbors(std::make_pair(row - 1, accumulatedEnergy[row - 1][column - 1].second), std::make_pair(row, accumulatedEnergy[row][column - 1].second), std::make_pair(row + 1, accumulatedEnergy[row + 1][column - 1].second));
        unsigned m = energyInPixel + min.second;
        accumulatedEnergy[row][column] = std::make_pair(min.first, m);
      }
    }
  }
  return accumulatedEnergy;
}

std::pair<std::size_t, unsigned> getMinEnergyInPixel(std::vector<std::vector<std::pair<std::size_t, long>>> cumulative_energy) noexcept
{
  long row_size = cumulative_energy.size();
  long column_size = cumulative_energy[0].size();

  long min_energy = cumulative_energy[0][column_size - 1].second;
  size_t index =  cumulative_energy[0][column_size - 1].first;

  for (int i = 0; i < row_size; i++)
  {
    if (cumulative_energy[i][column_size - 1].second <= min_energy)
    {
      min_energy = cumulative_energy[i][column_size - 1].second;
      index = i;
      //std::cout<<"| index: "<<index<<" min_enery: "<<min_energy;
    }
  }
  return std::make_pair(index, min_energy);
}
// std::pair<std::size_t, unsigned>
// pre_index, current_index, min_energy
std::tuple<size_t, size_t, long> getMinEnergyInPixelPath(std::vector<std::vector<std::pair<std::size_t, long>>> cumulative_energy) noexcept
{
  long row_size = cumulative_energy.size();
  long column_size = cumulative_energy[0].size();

  size_t current_index = 0;
  size_t pre_index =  cumulative_energy[0][column_size - 1].first;
  long min_energy = cumulative_energy[0][column_size - 1].second;

  for (int i = 0; i < row_size; i++)
  {
    if (cumulative_energy[i][column_size - 1].second < min_energy)
    {
      current_index = i;
      min_energy = cumulative_energy[i][column_size - 1].second;
      pre_index = cumulative_energy[i][column_size - 1].first;
      //std::cout<<"| index: "<<index<<" min_enery: "<<min_energy;
    }
  }
  return std::make_tuple(pre_index, current_index, min_energy);
}


std::pair<std::vector<std::size_t>, unsigned> optimalPath(std::vector<std::vector<std::pair<std::size_t, long>>> cumulative_energy) noexcept
{
  long row_size = cumulative_energy.size();
  long column_size = cumulative_energy[0].size();

  std::vector<std::size_t> optimalPixel(column_size,0);
  std::tuple<size_t, size_t, long> min = getMinEnergyInPixelPath(cumulative_energy);
  size_t next_index = std::get<0>(min);
  optimalPixel[column_size -1] = std::get<1>(min);
  long min_energy_value = std::get<2>(min);

  for (int i = column_size - 2; i >= 0; i--)
  {
    optimalPixel[i] = next_index;
    next_index = cumulative_energy[next_index][i].first; 
    //std::cout<< optimalPixel[i-1]<<"/";
  }
  return std::make_pair(optimalPixel, min_energy_value);
}