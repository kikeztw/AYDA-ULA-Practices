#include <iostream>
#include <string>

#include <Carve.hpp>
#include <Color.hpp>
#include <Png.hpp>

using namespace std;

vector<vector<Color>> read_image(const string& filename)
{
  Png input{filename};
  vector<vector<Color>> pixels;
  
  cout << "Reading image\n";
  if (!input.read(pixels))
  {
    cerr << "Could not read the file " << filename << endl;
    abort();
  }
  cout << "Done\n";

  return pixels;
}

void write_image(const string& filename, const vector<vector<Color>>& pixels, size_t n)
{
  auto dot_position = filename.find_last_of('.');
  string fn{filename.substr(0, dot_position) + string{"-horizontal-resized-"} + to_string(n) + filename.substr(dot_position)};

  Png output{fn};
  
  cout << "Writing image\n";
  if (!output.write(pixels))
  {
    cerr << "Could not write the file " << filename << endl;
    abort();
  }

  cout << "Created the file " << fn << " successfully\n";
}

int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    cout << "Usage: " << argv[0] << " input num_seams_to_remove\n";
    return EXIT_FAILURE;
  }

  auto pixels = read_image(argv[1]);

  auto n = std::stoul(argv[2]);

  Carve carve{pixels};

  cout << "Removing the " << n << " lowest seams\n";
  auto resized_pixels = carve.remove_n_lowest_seams(n);

  write_image(argv[1], resized_pixels, n);

  cout << "Everything ok!" << endl;
  return EXIT_SUCCESS;
}
