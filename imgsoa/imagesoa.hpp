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

  // Get the number of pixels
  size_t size() const { return r.size(); }

  // Resize all components
  void resize(size_t n) {
      r.resize(n);
      g.resize(n);
      b.resize(n);
    }

  // Access a pixel as a tuple
  std::tuple<T, T, T> getPixel(size_t i) const {
      return std::make_tuple(r[i], g[i], b[i]);
    }

  // Set a pixel at a given index
  void setPixel(size_t i, const std::tuple<T, T, T>& pixel) {
      r[i] = std::get<0>(pixel);
      g[i] = std::get<1>(pixel);
      b[i] = std::get<2>(pixel);
    }
};

template <typename T>
struct Pixel_map {
  std::size_t operator()(ImageSOA<T> const & color) const {
    return std::hash<T>{}(get<0>(color)) ^ std::hash<T>{}(get<1>(color) ^ std::hash<T>{}(get<2>(color)));
  }
  bool operator()(const std::tuple<T, T, T>& a, const std::tuple<T, T, T>& b) const {
    return std::tie(std::get<0>(a), std::get<1>(a), std::get<2>(a)) <
           std::tie(std::get<0>(b), std::get<1>(b), std::get<2>(b));
  }
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