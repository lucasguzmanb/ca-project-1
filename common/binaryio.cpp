#include "binaryio.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

Metadata obtainMetadata(std::ifstream & inputFile) {
  /*
   * get metadata of the picture and return it
   */
  Metadata metadata;
  inputFile >> metadata.format;
  inputFile >> metadata.width >> metadata.height >> metadata.maxColorValue;
  inputFile.ignore(1, '\n');
  return metadata;
}

std::ifstream openInputFile(std::filesystem::path const & fileName) {
  /*
   * open file by name and check if it was opened fine
   * return input file stream(file descriptor)
   */
  std::ifstream inputFile(fileName, std::ios::binary);
  // check if file can be opened
  if (!inputFile.is_open()) {
    std::cerr << "Error: can't open file " << fileName << '\n';
    exit(EXIT_FAILURE);
  }
  return inputFile;
}

std::ofstream openOutputFile(std::filesystem::path const & fileName) {
  /*
   * open file by name and check if it was opened fine
   * return output file stream(file descriptor)
   */
  std::ofstream outputFile(fileName, std::ios::binary);
  // check if file can be opened
  if (!outputFile.is_open()) {
    std::cerr << "Error: can't open file " << fileName << '\n';
    exit(EXIT_FAILURE);
  }
  return outputFile;
}

void writeMetadata(std::ofstream & outputFile, Metadata const & metadata) {
  /*
   * write metadata provided into output file passed by file stream
   */
  outputFile << metadata.format << '\n'
             << metadata.width << ' ' << metadata.height << '\n'
             << metadata.maxColorValue << '\n';
}

void writeMetadataCPPM(std::ofstream & outputFile, Metadata const & metadata, int const numColors) {
  /*
   * write metadata provided into output file passed by file stream
   */
  outputFile << metadata.format << ' ' << metadata.width << ' ' << metadata.height << ' '
             << metadata.maxColorValue << ' ' << numColors << '\n';
}

void checkFileFormat(std::string const & format) {
  /*
   * check if file is in P6 format
   */
  if (format != "P6") {
    std::cerr << "Error: file is not in .ppm format\n";
    exit(EXIT_FAILURE);
  }
}