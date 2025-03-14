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

    bool operator<(Pixel const & other) const {
      if (r != other.r) { return r < other.r; }
      if (g != other.g) { return g < other.g; }
      return b < other.b;
    }
};

template <typename T>
struct Pixel_map {
    std::size_t operator()(Pixel<T> const & color) const {
      return std::hash<T>{}(color.r) ^ std::hash<T>{}(color.g) ^ std::hash<T>{}(color.b);
    }

    bool operator()(Pixel<T> const & pixel1, Pixel<T> const & pixel2) const {
      if (pixel1.r != pixel2.r) { return pixel1.r < pixel2.r; }
      if (pixel1.g != pixel2.g) { return pixel1.g < pixel2.g; }
      return pixel1.b < pixel2.b;
    }
};

template <typename T>
std::vector<Pixel<T>> binaryToAOS(std::ifstream & inputFile, int width, int height);

template <typename T>
void AOSToBinary(std::ofstream & outputFile, std::vector<Pixel<T>> const & data);

template <typename T>
void AOSToBinary_(std::ofstream & outputFile, T const & data);



#endif  // IMAGEAOS_HPP