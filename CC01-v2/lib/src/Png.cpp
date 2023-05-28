#include <vector>

#include <png.h>


#include <Color.hpp>
#include <Png.hpp>

Png::Png(const std::string& _filename) noexcept
  : filename(_filename)
{
    
}

bool Png::read(std::vector<std::vector<Color>>& pixels) const noexcept
{
  FILE* fp = fopen(filename.c_str(), "rb");

  if (!fp)
  {
    return false;
  }

  auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

  if (!png_ptr)
  {
    fclose(fp);
    return false;
  }
  
  auto png_info = png_create_info_struct(png_ptr);
  
  if (!png_info)
  {
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    fclose(fp);
    return false;
  }

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_read_struct(&png_ptr, &png_info, nullptr);
    fclose(fp);
    return false;
  }

  png_init_io(png_ptr, fp);
  png_read_info(png_ptr, png_info);

  png_uint_32 width, height;
  int bit_depth, color_type;
  png_get_IHDR(png_ptr, png_info, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);

  if (bit_depth == 16)
  {
    png_set_strip_16(png_ptr);
  }

  if (color_type == PNG_COLOR_TYPE_PALETTE)
  {
    png_set_palette_to_rgb(png_ptr);
  }

  png_bytep* rows = (png_bytep*)malloc(sizeof(png_bytep) * height);
 
  for (int i = 0; i < height; ++i)
  {
    rows[i] = (png_byte*) malloc(png_get_rowbytes(png_ptr, png_info));
  }

  png_read_image(png_ptr, rows);

  png_destroy_read_struct(&png_ptr, &png_info, nullptr);

  fclose(fp);

  for (png_uint_32 i = 0; i < height; ++i)
  {
    pixels.push_back(std::vector<Color>{});
    auto& pixels_row = pixels.back();
    png_byte* row = rows[i];

    for (png_uint_32 j = 0; j < width; ++j)
    {
      png_byte* ptr = &(row[j*3]);
      pixels_row.push_back(Color(ptr[0], ptr[1], ptr[2]));
    }

    free(row);
  }

  free(rows);

  return true;
}


bool Png::write(const std::vector<std::vector<Color>>& pixels) const noexcept
{
  FILE* fp = fopen(filename.c_str(), "wb");
  
  if (!fp)
  {
    return false;
  }
  
  auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

  if (!png_ptr)
  {
    fclose(fp);
    remove(filename.c_str());
    return false;
  }
  
  auto png_info = png_create_info_struct(png_ptr);
  
  if (!png_info)
  {
    png_destroy_write_struct(&png_ptr, nullptr);
    fclose(fp);
    remove(filename.c_str());
    return false;
  }
  
  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_write_struct(&png_ptr, &png_info);
    fclose(fp);
    remove(filename.c_str());
    return false;
  }

  png_init_io(png_ptr, fp);
  
  const auto width{pixels[0].size()};
  const auto height{pixels.size()};
  
  png_set_IHDR(png_ptr, png_info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, png_info);
  
  
  png_bytep* rows = (png_bytep*) malloc(sizeof(png_bytep)*height);
  
  for (auto i = 0; i < height; ++i)
  {
    rows[i] = (png_byte*) malloc(png_get_rowbytes(png_ptr, png_info));
    png_byte* row = rows[i];
    
    for (auto j = 0; j < width; ++j)
    {
      Color pixel = pixels[i][j];
      png_byte* ptr = &(row[j*3]);
      
      ptr[0] = pixel.get_red();
      ptr[1] = pixel.get_green();
      ptr[2] = pixel.get_blue();
    }
  }
  
  png_write_image(png_ptr, rows);    
  png_write_end(png_ptr, nullptr);
  
  for (auto i = 0; i < height; ++i)
  {
    free(rows[i]);
  }
  free(rows);
  
  png_destroy_write_struct(&png_ptr, &png_info);
  fclose(fp);

  return true;
}
