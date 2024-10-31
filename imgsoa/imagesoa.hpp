/* Support for image processing with SOA (headers) */
#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP
#include "common/binaryio.hpp"

#include <fstream>
#include <variant>
#include <vector>

template <typename T>
struct ImageSOA {
    std::vector<T> r, g, b;

    explicit ImageSOA(size_t size = 0) : r(size), g(size), b(size) {}
};

void writeBinaryData(std::ofstream & outputFile,
                     std::variant<ImageSOA<uint8_t>, ImageSOA<uint16_t>> const & outputPixels);

template <typename T>
ImageSOA<T> binaryToSOA(std::ifstream & inputFile, int const width, int const height) {
  auto dataSize = static_cast<std::size_t>(width * height);
  ImageSOA<T> data;
  data.r.resize(dataSize);
  data.g.resize(dataSize);
  data.b.resize(dataSize);
  for (std::size_t i = 0; i < dataSize; ++i) {
    data.r[i] = readBinary<T>(inputFile);
    data.g[i] = readBinary<T>(inputFile);
    data.b[i] = readBinary<T>(inputFile);
  }
  return data;
}

template <typename T>
void SOAToBinary(std::ofstream & outputFile, ImageSOA<T> const & data) {
  for (std::size_t i = 0; i < data.r.size(); ++i) {
    writeBinary(outputFile, data.r[i]);
    writeBinary(outputFile, data.g[i]);
    writeBinary(outputFile, data.b[i]);
  }
}

#endif  // IMAGESOA_HPP