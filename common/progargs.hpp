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

void check_minimum_args(int argc);

Arguments parse_args(std::vector<std::string> const & argv);

void checkArgsOperation(Arguments const & args);

void checkArgsExtraEmpty(Arguments const & args);

void checkArgsExtra1(Arguments const & args);

void checkArgsExtra2(Arguments const & args);

void check_args(Arguments const & args);

Arguments checkAndParseArgs(const std::vector<std::string>& args_vector);


#endif // PROGARGS_HPP