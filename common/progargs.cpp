
#include "progargs.hpp"

#include <iostream>
#include <string>
#include <vector>

void check_minimum_args(int const argc) {
  /*
   * check the amount of arguments passed to the program
   */
  if (argc < 4) {
    std::cerr << "Error: invalid number of arguments: " << argc - 1 << "\n";
    exit(EXIT_FAILURE);
  }
}

Arguments parse_args(std::vector<std::string> const & argv) {
  /*
   * by the vector of arguments, create object of arguments
   * object stores input and output filenames, operation to be performed
   * and also additional parameters(depends on each function)
   */
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
  /*
   * check if the provided program parameters are correct and can be used or not
   */
  if (args.operation != "info" && args.operation != "maxlevel" && args.operation != "resize" &&
      args.operation != "cutfreq" && args.operation != "compress") {
    std::cerr << "Error: invalid operation: " << args.operation << "\n";
    exit(EXIT_FAILURE);
  }
  if (args.extra.empty()) {
    if (args.operation != "info" && args.operation != "compress") {
      std::cerr << "Error: invalid number of extra arguments for " << args.operation << ": "
                << args.extra.size() << "\n";
      exit(EXIT_FAILURE);
    }
  } else if (args.extra.size() == 1) {
    if (args.operation != "maxlevel" && args.operation != "cutfreq") {
      std::cerr << "Error: invalid number of extra arguments for " << args.operation << ": "
                << args.extra.size() << "\n";
      exit(EXIT_FAILURE);
    } else if (args.operation == "maxlevel" && (args.extra[0] < 0 || args.extra[0] > 65535)) {
      std::cerr << "Error: invalid maxlevel: " << args.extra[0] << "\n";
      exit(EXIT_FAILURE);
    } else if (args.operation == "cutfreq" && args.extra[0] <= 0) {
      std::cerr << "Error: invalid cutfreq: " << args.extra[0] << "\n";
      exit(EXIT_FAILURE);
    }
  } else if (args.extra.size() == 2) {
    if (args.operation != "resize") {
      std::cerr << "Error: invalid number of extra arguments for " << args.operation << ": "
                << args.extra.size() << "\n";
      exit(EXIT_FAILURE);
    } else if (args.extra[0] <= 0) {
      std::cerr << "Error: invalid resize width: " << args.extra[0] << "\n";
      exit(EXIT_FAILURE);
    } else if (args.extra[1] <= 0) {
      std::cerr << "Error: invalid resize height: " << args.extra[1] << "\n";
      exit(EXIT_FAILURE);
    }
  } else {
    std::cerr << "Error: invalid number of extra arguments for " << args.operation << ": "
              << args.extra.size() << "\n";
    exit(EXIT_FAILURE);
  }
}

Arguments checkAndParseArgs(const std::vector<std::string>& args_vector) {
  /*
   * check is provided parameters are correct and create object of arguments
   * return this object
   */
  Arguments args = parse_args(args_vector);
  check_args(args);
  return args;
}