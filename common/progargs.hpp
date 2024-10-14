/* Header containing functions for argument validation and parsing */
#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include <string>
#include <vector>
#include <filesystem>

struct Arguments {
  std::filesystem::path input, output;
  std::string operation;
  std::vector<int> extra;
};

void print_arguments(Arguments const & arguments);

void check_minimum_args(int argc);

Arguments parse_args(std::vector<std::string> const & argv);

void check_args(Arguments const & args);


#endif // PROGARGS_HPP