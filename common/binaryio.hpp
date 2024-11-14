#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

struct Metadata {
    std::string format;
    int width;
    int height;
    int maxColorValue;

    // Constructor
    Metadata(std::string _format, int const _width, int const _height, int const _maxColorValue)
      : format(std::move(_format)), width(_width), height(_height), maxColorValue(_maxColorValue) {
    }

    // Default constructor
    Metadata() : width(0), height(0), maxColorValue(0) { }
};

Metadata obtainMetadata(std::ifstream & inputFile);

std::ifstream openInputFile(std::filesystem::path const & fileName);
std::ofstream openOutputFile(std::filesystem::path const & fileName);

void writeMetadata(std::ofstream & outputFile, Metadata const & metadata);

void writeMetadataCPPM(std::ofstream & outputFile, Metadata const & metadata, int numColors);

template <typename T>
std::vector<T> readRawData(std::ifstream & inputFile, int const width, int const height) {
  /*
   * by the input file stream and its size
   * create vector that stores the sequence of pixels of picture and return it
   */
  // Create a vector of raw data to store the binary data and read everything at once
  auto dataSize = static_cast<std::size_t>(width * height * 3);
  std::vector<T> tempData(dataSize);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  inputFile.read(reinterpret_cast<char *>(tempData.data()),
                 static_cast<std::streamsize>(dataSize * sizeof(T)));
  return tempData;
}

#endif  // BINARYIO_HPP