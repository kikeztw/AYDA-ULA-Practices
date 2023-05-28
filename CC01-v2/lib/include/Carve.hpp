#pragma once

#include <vector>

class Color;

/**
 * @brief The third and final step in the seam carving process: removing the lowest-energy seam from an image.
 * 
 * By doing so iteratively, the size of the image can be reduced (in one dimension) by multiple pixels.
 * 
 * This class contains the operations to remove the n-lowest seams of an image. * 
 */
class Carve
{
public:
  Carve(const std::vector<std::vector<Color>>& _pixels) noexcept;

  /**
   * @brief Iteratively: Find the lowest-energy seam in the image and remove that seam from the image.
   * 
   * Repeat this process `num_seams_to_remove` times, so that the resulting image has that many pixels removed in each row.

   * @param num_seams_to_remove 
   * @return The resulting matrix of pixels.
   */
  std::vector<std::vector<Color>> remove_n_lowest_seams(std::size_t num_seams_to_remove) const noexcept;

  /**
   * @brief Remove pixels from the image, as indicated by each of the i-coordinates in the input.
   * 
   * The i-coordinates are specified from top to bottom and span the entire height of the image.
   * 
   * @param pixels The matrix of pixels
   * @param seam_cols The list of i-coordinates forming the lowest-energy seam.
   * @return The new matrix of pixels after removing the lowest-energy seam. 
   */
  std::vector<std::vector<Color>> remove_seam(const std::vector<std::vector<Color>>& pixels, const std::vector<std::size_t>& seam_rows) const noexcept;

private:
  const std::vector<std::vector<Color>>& pixels;
};
