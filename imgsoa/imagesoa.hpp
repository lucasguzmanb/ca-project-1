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

    explicit ImageSOA(size_t size = 0) : r(size), g(size), b(size) { }
};

template <typename T>
ImageSOA<T> binaryToSOA(std::ifstream & inputFile, int const width, int const height) {

  std::vector<T> tempData = readRawData<T>(inputFile, width, height);
  ImageSOA<T> data(static_cast<std::size_t>(width * height));

  // Convert the raw data to SOA
  for (std::size_t i = 0, j = 0; i < data.r.size(); ++i, j += 3) {
    data.r[i] = tempData[j];
    data.g[i] = tempData[j + 1];
    data.b[i] = tempData[j + 2];
  }

  return data;
}

template <typename T>
void SOAToBinary(std::ofstream & outputFile, ImageSOA<T> const & data) {
  // Convert the SOA data to raw data
  std::vector<T> tempData(data.r.size() * 3);

  for (std::size_t i = 0, j = 0; i < data.r.size(); ++i, j += 3) {
    tempData[j]     = data.r[i];
    tempData[j + 1] = data.g[i];
    tempData[j + 2] = data.b[i];
  }

  // Write the raw data to the output file
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputFile.write(reinterpret_cast<char const *>(tempData.data()),
                   static_cast<std::streamsize>(tempData.size() * sizeof(T)));
}

#endif  // IMAGESOA_HPP