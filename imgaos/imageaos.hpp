/* Library for support for image processing with AOS */
#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP

#include "common/binaryio.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <tuple>
#include <variant>
#include <vector>

template <typename T>
struct Pixel {
  T r, g, b;

  bool operator==(Pixel<T> const & other) const {
    return std::tie(r, g, b) == std::tie(other.r, other.g, other.b);
  }
  bool operator<(Pixel<T> const & other) const {
    if (r != other.r) { return r < other.r; }
    if (g != other.g) { return g < other.g; }
    return b < other.b;
  }
    bool operator<(const Pixel &other) const {
      if (r != other.r) {return r < other.r;}
      if (g != other.g) {return g < other.g;}
      return b < other.b;

    }
};
      template <typename T>
struct Pixel_map {
    std::size_t operator()(Pixel<T> const & color) const {
      return std::hash<T>{}(color.r) ^ std::hash<T>{}(color.g) ^ std::hash<T>{}(color.b);
    }
  bool operator()(const Pixel<T>& a, const Pixel<T>& b) const {
      if (a.r != b.r) return a.r < b.r;
      if (a.g != b.g) return a.g < b.g;
      return a.b < b.b;
    }
};

template <typename T>
std::vector<Pixel<T>> binaryToAOS(std::ifstream & inputFile, int const width, int const height) {

  std::vector<T> tempData = readRawData<T>(inputFile, width, height);
  std::vector<Pixel<T>> data(static_cast<std::size_t>(width * height));

  // Convert the raw data to AOS
  for (std::size_t i = 0, j = 0; i < data.size(); ++i, j += 3) {
    data[i].r = tempData[j];
    data[i].g = tempData[j + 1];
    data[i].b = tempData[j + 2];
  }

  return data;
}

template <typename T>
void AOSToBinary(std::ofstream & outputFile, std::vector<Pixel<T>> const & data) {
  // Convert the AOS data to raw data
  std::vector<T> tempData(data.size() * 3);

  for (std::size_t i = 0, j = 0; i < data.size(); ++i, j += 3) {
    tempData[j]     = data[i].r;
    tempData[j + 1] = data[i].g;
    tempData[j + 2] = data[i].b;
  }

  // Write the raw data to the binary file
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputFile.write(reinterpret_cast<char const *>(tempData.data()),
                   static_cast<std::streamsize>(tempData.size() * sizeof(T)));
}


template <typename T>
void AOSToBinary_(std::ofstream &outputFile, const T& data) {
  // Write the single instance of T in binary to the output file
  outputFile.write(reinterpret_cast<const char*>(&data), sizeof(T));
}



#endif  // IMAGEAOS_HPP