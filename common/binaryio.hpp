#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

struct Metadata {
    std::string format;
    int width         = 0;
    int height        = 0;
    int maxColorValue = 0;

    Metadata() = default;
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
  auto dataSize = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 3;
  std::vector<T> tempData(dataSize);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  inputFile.read(reinterpret_cast<char *>(tempData.data()),
                 static_cast<std::streamsize>(dataSize * sizeof(T)));
  return tempData;
}

void checkFileFormat(std::string const & format);

#endif  // BINARYIO_HPP