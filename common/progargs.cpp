
#include "progargs.hpp"

#include <complex>
#include <iostream>
#include <string>
#include <vector>

void print_arguments(Arguments const & arguments) {
  std::cout << "input: " << arguments.input << "\n";
  std::cout << "output: " << arguments.output << "\n";
  std::cout << "operation: " << arguments.operation << "\n";
  std::cout << "extra arguments:\n";
  for (auto x : arguments.extra) { std::cout << x << std::endl; }
}

void check_minimum_args(int const argc) {
  if (argc < 4) {
    std::cerr << "Error: invalid number of arguments: " << argc - 1 << "\n";
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
      std::cerr << "Error: invalid extra arguments for operation " << args.operation << ": "
                << argv[i] << "\n";
      exit(EXIT_FAILURE);
    }
  }
  return args;
}

void check_args(Arguments const & args) {
  if (args.operation == "info") {
    if (!args.extra.empty()) {  // info doesn't take extra arguments
      std::cerr << "Error: invalid number of extra arguments for info: " << args.extra.size()
                << "\n";
      exit(EXIT_FAILURE);
    }
  } else if (args.operation == "maxlevel") {
    if (args.extra.size() != 1) {  // maxlevel takes exactly one extra argument
      std::cerr << "Error: invalid number of extra arguments for maxlevel: " << args.extra.size()
                << "\n";
      exit(EXIT_FAILURE);
    } else if (args.extra[0] < 0 ||
               args.extra[0] > 65535) {  // maxlevel's extra argument must be between 0 and 65535
      std::cerr << "Error: invalid maxlevel: " << args.extra[0] << "\n";
      exit(EXIT_FAILURE);
    }
  } else if (args.operation == "resize") {
    if (args.extra.size() != 2) {
      std::cerr << "Error: invalid number of extra arguments for resize: " << args.extra.size()
                << "\n";
      exit(EXIT_FAILURE);
    } else if (args.extra[0] <= 0) {
      std::cerr << "Error: invalid resize width: " << args.extra[0] << "\n";
      exit(EXIT_FAILURE);
    } else if (args.extra[1] <= 0) {
      std::cerr << "Error: invalid resize height: " << args.extra[1] << "\n";
      exit(EXIT_FAILURE);
    }
  } else if (args.operation == "cutfreq") {
    if (args.extra.size() != 1) {
      std::cerr << "Error: invalid number of extra arguments for cutfreq: " << args.extra.size()
                << "\n";
      exit(EXIT_FAILURE);
    } else if (args.extra[0] <= 0) {
      std::cerr << "Error: invalid cutfreq: " << args.extra[0] << "\n";
      exit(EXIT_FAILURE);
    }
  } else if (args.operation == "compress") {
    if (!args.extra.empty()) {
      std::cerr << "Error: invalid number of extra arguments for compress: " << args.extra.size()
                << "\n";
      exit(EXIT_FAILURE);
    }
  } else {
    std::cerr << "Error: invalid operation: " << args.operation << "\n";
    exit(EXIT_FAILURE);
  }
}