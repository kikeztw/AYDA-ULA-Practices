#pragma once

#include <cstdio>
#include <string>

class Color;

class Png
{
public:
  Png(const std::string& filename) noexcept;

  bool read(std::vector<std::vector<Color>>& pixels) const noexcept;

  bool write(const std::vector<std::vector<Color>>& pixels) const noexcept;

private:
  std::string filename;
};
