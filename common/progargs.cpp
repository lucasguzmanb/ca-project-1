
#include "progargs.hpp"

#include <iostream>
#include <string>
#include <vector>

void print_arguments(Arguments arguments) {
  std::cout << "input: " << arguments.input << "\n";
  std::cout << "output: " << arguments.output << "\n";
  std::cout << "operation: " << arguments.operation << "\n";
  for (auto x : arguments.extra) {
    std::cout << x << std::endl;
  }
}

void check_minimum_args(int const argc) {
  if (argc < 4) {
    std::cerr << "Error: invalid number of arguments: " << argc - 1 << "\n";
    std::cerr << "Usage: ./imtool-<soa/aos> <input> <output> <operation> <[extra arguments...]>\n";
    exit(EXIT_FAILURE);
  }
}

Arguments parse_args(std::vector<std::string> const & argv) {
  Arguments args;
  args.input     = argv[0];
  args.output    = argv[1];
  args.operation = argv[2];
  for (size_t i = 3; i < argv.size(); ++i) {
    try {
      // Try to convert the parameter to integer
      int num = std::stoi(argv[i]);
      if (std::to_string(num) != argv[i]) {
        // To ensure arguments like "123hello" aren't valid since stoi returns 123
        throw std::invalid_argument("Argument is not a valid integer");
      }
      args.extra.push_back(num);
    } catch (std::invalid_argument const &) {
      // If it's not a valid argument, exit with error
      std::cerr << "Error: invalid extra arguments for operation " << args.operation << ": " << argv[i]<< "\n";
      exit(EXIT_FAILURE);
    }
  }
  return args;
}