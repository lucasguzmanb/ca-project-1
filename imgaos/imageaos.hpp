/* Library for support for image processing with AOS */
#ifndef IMAGEAOS_HPP
#define IMAGEAOS_HPP
#include <common/binaryio.hpp>
#include <cstddef>
#include <fstream>
#include <vector>

template <typename T>
struct Pixel {
    T r, g, b;
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
void AOSToBinary(std::ofstream & outputFile, std::vector<Pixel<T>> const & data) {
  for (auto const & pixel : data) {
    write_binary<T>(outputFile, pixel.r);
    write_binary<T>(outputFile, pixel.g);
    write_binary<T>(outputFile, pixel.b);
  }
}
#endif  // IMAGEAOS_HPP