#include <Carve.hpp>
#include <Color.hpp>
#include <Energy.hpp>
#include <Seam.hpp>

Carve::Carve(const std::vector<std::vector<Color>>& _pixels) noexcept
  : pixels(_pixels)
{
  
}

std::vector<std::vector<Color>> Carve::remove_n_lowest_seams(size_t num_seams_to_remove) const noexcept
{
  std::vector<std::vector<Color>> new_image(pixels);

  for (int num = 0; num < num_seams_to_remove; num++)
  {
    std::vector<std::vector<unsigned>> energy_matrix = Energy(new_image).compute_energy();
    std::vector<std::size_t> optimal_path = Seam(energy_matrix).compute_horizontal_seams().first;
    new_image = remove_seam(new_image, optimal_path);
  }

  return new_image;
}

std::vector<std::vector<Color>> Carve::remove_seam(const std::vector<std::vector<Color>>& image, const std::vector<size_t>& seam_rows) const noexcept
{
  int height = image.size();
  int width = image[0].size();

  std::vector<std::vector<Color>> new_image(height - 1);
  
  for (int j = 0; j < width; j++)
  {
    int k = 0;

    for (int i = 0; i < height - 1; i++)
    {
      new_image[i].push_back((seam_rows[j] == i) ? image[i + 1][j] : (k != 0) ? image[i + 1][j] : image[i][j]);
      k = (seam_rows[j] == i) ? 1 : k;
    }
  }

  return new_image;
}

