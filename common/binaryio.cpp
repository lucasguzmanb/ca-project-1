#include "binaryio.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

Metadata obtainMetadata(std::ifstream & inputFile) {
  Metadata metadata;
  inputFile >> metadata.format;
  inputFile >> metadata.width >> metadata.height >> metadata.maxColorValue;
  inputFile.ignore(1, '\n');
  return metadata;
}

std::ifstream openInputFile(std::filesystem::path const & fileName) {
  std::ifstream inputFile(fileName, std::ios::binary);
  // check if file can be opened
  if (!inputFile.is_open()) {
    std::cerr << "Error: can't open file " << fileName << '\n';
    exit(EXIT_FAILURE);
  }
  return inputFile;
}

std::ofstream openOutputFile(std::filesystem::path const & fileName) {
  std::ofstream outputFile(fileName, std::ios::binary);
  // check if file can be opened
  if (!outputFile.is_open()) {
    std::cerr << "Error: can't open file " << fileName << '\n';
    exit(EXIT_FAILURE);
  }
  return outputFile;
}

void writeMetadata(std::ofstream & outputFile, Metadata const & metadata, int const numColors) {
  outputFile << metadata.format << ' ' << metadata.width << ' ' << metadata.height << ' '
             << metadata.maxColorValue << ' ';

  if (numColors != -1) {
    outputFile << numColors << '\n';
  } else {
    outputFile << '\n';
  }
}