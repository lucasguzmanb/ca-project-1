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
};

template <typename T>
struct Pixel_map {
  std::size_t operator()(Pixel<T> const & color) const {
    return std::hash<T>{}(color.r) ^ std::hash<T>{}(color.g) ^ std::hash<T>{}(color.b);
  }
};

template <typename T>
std::vector<Pixel<T>> binaryToAOS(std::ifstream & inputFile, int width, int height) {
  auto dataSize = static_cast<std::size_t>(width * height);
  std::vector<Pixel<T>> data(dataSize);
  for (std::size_t i = 0; i < dataSize; ++i) {
    data[i].r = readBinary<T>(inputFile);
    data[i].g = readBinary<T>(inputFile);
    data[i].b = readBinary<T>(inputFile);
  }
  return data;
}

template <typename T>
void AOSToBinary(std::ofstream & outputFile, std::vector<Pixel<T>> const & data, int const width,
                 int const height) {
  auto dataSize = static_cast<std::size_t>(width * height);
  for (std::size_t i = 0; i < dataSize; ++i) {
    writeBinary(outputFile, data[i].r);
    writeBinary(outputFile, data[i].g);
    writeBinary(outputFile, data[i].b);
  }
}

#endif  // IMAGEAOS_HPP