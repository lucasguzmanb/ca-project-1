#include "binaryio.hpp"

#include <fstream>

Metadata obtainMetadata(std::ifstream & inputFile) {
  Metadata metadata;
  inputFile >> metadata.format;
  inputFile >> metadata.width >> metadata.height >> metadata.maxColorValue;
  inputFile.ignore(1, '\n');
  return metadata;
}