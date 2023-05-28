#include <iostream>

#include <Color.hpp>
#include <Energy.hpp>
#include <Seam.hpp>
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

  return pixels;
}

void write_image(const string& filename, const vector<vector<Color>>& pixels)
{
  auto dot_position = filename.find_last_of('.');
  string fn{filename.substr(0, dot_position) + string{"-seam"} + filename.substr(dot_position)};

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
  if (argc < 2)
  {
    cout << "Usage: " << argv[0] << " input\n";
    return EXIT_FAILURE;
  }

  auto pixels = read_image(argv[1]);

  Energy energy(pixels);
  auto computed_energy = energy.compute_energy();
  Seam seam(computed_energy);
  auto computed_seam = seam.compute_horizontal_seam();


  write_image(argv[1], Seam::min_seam_to_colors(pixels, computed_seam.first));

  cout << "Everything ok!" << endl;
  return EXIT_SUCCESS;
}
