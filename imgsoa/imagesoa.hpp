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
    [[nodiscard]] size_t size() const { return r.size(); }

    // Resize all components
    void resize(size_t n) {
      r.resize(n);
      g.resize(n);
      b.resize(n);
    }

    // Access a pixel as a tuple
    [[nodiscard]] std::tuple<T, T, T> getPixel(size_t index) const {
      return std::make_tuple(r[index], g[index], b[index]);
    }

    // Set a pixel at a given index
    void setPixel(size_t index, const std::tuple<T, T, T>& pixel) {
      r[index] = std::get<0>(pixel);
      g[index] = std::get<1>(pixel);
      b[index] = std::get<2>(pixel);
    }
};

template <typename T>
ImageSOA<T> binaryToSOA(std::ifstream & inputFile, int width, int height);


template <typename T>
void SOAToBinary(std::ofstream & outputFile, ImageSOA<T> const & data);

template <typename T>
void SOAToBinary_(std::ofstream & outputFile, T const & data) {
  // Write the single instance of T in binary to the output file
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputFile.write(reinterpret_cast<char const *>(&data), sizeof(T));
}

#endif  // IMAGESOA_HPP