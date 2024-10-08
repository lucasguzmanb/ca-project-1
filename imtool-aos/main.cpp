/* AOS */
#include "common/progargs.hpp"

#include <iostream>
#include <span>
#include <vector>

int main(int const argc, char * argv[]) {
  // check parameters
  std::vector<std::string> args;
  if (argc > 1) {
    std::span<char *> args_view(argv + 1, std::size_t(argc - 1));
    args = {args_view.begin(), args_view.end()};
    print_arguments(args);
  } else {
    std::cout << "No arguments provided." << std::endl;
  }

  print_arguments(args);

  return 0;
}