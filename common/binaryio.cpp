#include "binaryio.hpp"

#include <fstream>
#include <iostream>

Metadata obtainMetadata(std::ifstream & inputFile) {
  Metadata metadata;
  inputFile >> metadata.format;
  inputFile >> metadata.width >> metadata.height >> metadata.maxColorValue;
  inputFile.ignore(1, '\n');
  return metadata;
}

std::ifstream openInputFile(std::string const & fileName) {
  std::ifstream inputFile(fileName, std::ios::binary);
  // check if file can be opened
  if (!inputFile.is_open()) {
    std::cerr << "Error: can't open file " << fileName << '\n';
    exit(-1);
  }
  return inputFile;
}

std::ofstream openOutputFile(std::string const & fileName) {
  std::ofstream outputFile(fileName, std::ios::binary);
  // check if file can be opened
  if (!outputFile.is_open()) {
    std::cerr << "Error: can't open file " << fileName << '\n';
    exit(-1);
  }
  return outputFile;
}