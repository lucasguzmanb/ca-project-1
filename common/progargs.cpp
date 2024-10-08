
#include <iostream>
#include <string>
#include <vector>

void print_arguments(std::vector<std::string> const & arguments) {
  for (auto const & arg : arguments) { std::cout << arg << "\n"; }
}