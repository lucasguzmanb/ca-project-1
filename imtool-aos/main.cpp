/* AOS */
#include "common/progargs.hpp"
#include <iostream>
#include <span>
#include <vector>

int main(int const argc, char * argv[]) {
  // check & parse arguments
  check_minimum_args(argc);
  std::span<char *> args_view(argv + 1, std::size_t(argc - 1));
  std::vector<std::string> const args_vector = {args_view.begin(), args_view.end()};
  Arguments const args =  parse_args(args_vector);
  check_args(args);
  print_arguments(args);

  return 0;
}