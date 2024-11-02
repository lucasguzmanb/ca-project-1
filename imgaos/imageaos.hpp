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
std::vector<Pixel<T>> binaryToAOS(std::ifstream & inputFile, int const width, int const height) {
  // Create a vector of raw data to store the binary data and read everything at once
  auto dataSize = static_cast<std::size_t>(width * height * 3);
  std::vector<T> tempData(dataSize);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  inputFile.read(reinterpret_cast<char *>(tempData.data()),
                 static_cast<std::streamsize>(dataSize * sizeof(T)));

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

#endif  // IMAGEAOS_HPP