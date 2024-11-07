#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

struct Metadata {
    std::string format;
    int width;
    int height;
    int maxColorValue;
};

Metadata obtainMetadata(std::ifstream & inputFile);

std::ifstream openInputFile(std::filesystem::path const & fileName);
std::ofstream openOutputFile(std::filesystem::path const & fileName);

void writeMetadata(std::ofstream & outputFile, Metadata const & metadata);

template <typename T>
std::vector<T> readRawData(std::ifstream & inputFile, int const width, int const height) {
  // Create a vector of raw data to store the binary data and read everything at once
  auto dataSize = static_cast<std::size_t>(width * height * 3);
  std::vector<T> tempData(dataSize);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  inputFile.read(reinterpret_cast<char *>(tempData.data()),
                 static_cast<std::streamsize>(dataSize * sizeof(T)));
  return tempData;
}

#endif  // BINARYIO_HPP