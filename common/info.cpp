#include "info.hpp"

#include <iostream>

void info(Arguments const & args, Metadata const & metadata) {
  std::cout << "Input: " << args.input << '\n';
  std::cout << "Output: " << args.output << '\n';
  std::cout << "Operation: " << args.operation << '\n';
  std::cout << "Image size: " << metadata.width << "x" << metadata.height << '\n';
  std::cout << "Max level: " << metadata.maxColorValue << '\n';
}