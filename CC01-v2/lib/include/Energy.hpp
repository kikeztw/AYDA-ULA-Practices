#pragma once

#include <vector>

class Color;

/**
 * @brief The first step in the seam carving algorithm: computing the energy of an image.
 * 
 * This class contains the operations to compute and visualize the energy of an image.
 */
class Energy
{
public:
  Energy(const std::vector<std::vector<Color>>& _pixels) noexcept;

  std::vector<std::vector<unsigned>> compute_energy() const noexcept;

  /**
   * @brief Convert the energy values at each pixel into colors that can be used to visualize the energy of the image.
   * 
   * The steps to do this conversion are:
   * 1. Normalize the energy values to be between 0 and 255.
   * 2. Convert these values into grayscale colors, where the RGB values are all the same for a single color.
   * 
   * @param energy 
   * @return A matrix with the energy colors.
   */
  static std::vector<std::vector<Color>> energy_to_colors(const std::vector<std::vector<unsigned>>& energy);

  /**
   * @brief Compute the energy of the image at the given (i, j) position.
   * 
   * The energy of the pixel is determined by looking at the pixels surrounding
   * the requested position. In the case the requested position is at the edge
   * of the image, the current position is used whenever a "surrounding position"
   * would go out of bounds.
   * 
   * @param i the pixel row.
   * @param j the pixel column.
   * @return the energy of the pixel.
   */
  unsigned energy_at(size_t i, size_t j) const noexcept;

private:
  const std::vector<std::vector<Color>>& pixels;
};
