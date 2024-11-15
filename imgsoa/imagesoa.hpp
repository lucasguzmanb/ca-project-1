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
ImageSOA<T> binaryToSOA(std::ifstream & inputFile, int const width, int const height);


template <typename T>
void SOAToBinary(std::ofstream & outputFile, ImageSOA<T> const & data);

template <typename T>
void SOAToBinary_(std::ofstream & outputFile, T const & data) {
  // Write the single instance of T in binary to the output file
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputFile.write(reinterpret_cast<char const *>(&data), sizeof(T));
}

#endif  // IMAGESOA_HPP