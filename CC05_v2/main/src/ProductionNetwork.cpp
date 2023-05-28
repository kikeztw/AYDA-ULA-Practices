#include <bits/stdc++.h>

#include <Models.hpp>
#include <Generator.hpp>
#include <Rng.hpp>

#include <ProductionNetwork.hpp>

using namespace std;

int main()
{
  auto pd = NetworkGenerator::generate_random_production_data(3, 3, 2, 4, 0.1f, 0.1f);

  ProductionNetwork network{pd};

  std::ofstream dot("original_graph.dot");
  network.to_dot(dot);
  dot.close();
  
  cout << "First-level goods\n";
  cout << "Note that any of these goods could not belong to the network\n";

  for (auto p: pd.processes)
  {
    if (p->get_inputs_table().empty())
    {
      cout << p->get_output()->get_description() << endl;
    }
  }
  
  string good_name{};
  cout << "Enter the name of a first-level good: ";
  cin >> good_name;

  shared_ptr<Good> good{nullptr};


  for (auto p: pd.processes)
  {
    if (p->get_inputs_table().empty() && p->get_output()->get_description() == good_name)
    {
      good = p->get_output();
      break;
    }
  }

  if (good == nullptr)
  {
    cout << "The chosen good does not exist or it is not a first-level good\n";
    return EXIT_FAILURE;
  }

  float change{0.f};
  cout << "Enter the price change amount: ";
  cin >> change;

  change = max(0.f, change);

  try
  {
    auto result = network.change_price(good, change);
  
    cout << "The changes were\n";
    for (const auto r: result)
    {
      cout << "Seller company: " << get<0>(r)->get_rif() << endl;
      cout << "Buyer company: " << get<1>(r)->get_rif() << endl;
      cout << "Good: " << get<2>(r)->get_description() << endl;
      cout << "Old price: " << get<3>(r) << endl;
      cout << "New price: " << get<4>(r) << endl;
      cout << "Percentage of change: " << get<5>(r) << endl << endl;
    }

    dot.open("changed_graph.dot");
    network.to_dot(dot);
    dot.close();

    cout << "The files original_graph.dot and changed_graph.dot were generated on the current directory.\n";
    cout << "You can generate a view of them with the shell command dot -Tsvg file.dot -o file.svg\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
