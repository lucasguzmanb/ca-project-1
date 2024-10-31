#ifndef MAXLEVEL_SOA_HPP
#define MAXLEVEL_SOA_HPP

#include "imagesoa.hpp"

#include <cstdint>
#include <variant>

template <typename T, typename U>
ImageSOA<U> maxlevel(ImageSOA<T> inputImage, int const oldLevel, int const newLevel) {
        ImageSOA<U> outputImage(inputImage.r.size());
        for (size_t i = 0; i < inputImage.r.size(); ++i) {
        outputImage.r[i] = static_cast<U>((inputImage.r[i] * newLevel + oldLevel / 2) / oldLevel);
        outputImage.g[i] = static_cast<U>((inputImage.g[i] * newLevel + oldLevel / 2) / oldLevel);
        outputImage.b[i] = static_cast<U>((inputImage.b[i] * newLevel + oldLevel / 2) / oldLevel);
        }
        return outputImage;
}

template <typename T>
ImageSOA<uint8_t> maxlevel8(ImageSOA<T> const & inputImage, int const oldLevel,
                            int const newLevel) {
  ImageSOA<uint8_t> outputImage(inputImage.r.size());
  for (size_t i = 0; i < inputImage.r.size(); ++i) {
    outputImage.r[i] =
        static_cast<uint8_t>((inputImage.r[i] * newLevel + oldLevel / 2) / oldLevel);
    outputImage.g[i] =
        static_cast<uint8_t>((inputImage.g[i] * newLevel + oldLevel / 2) / oldLevel);
    outputImage.b[i] =
        static_cast<uint8_t>((inputImage.b[i] * newLevel + oldLevel / 2) / oldLevel);
  }
  return outputImage;
}

template <typename T>
ImageSOA<uint16_t> maxlevel16(ImageSOA<T> const & inputImage, int const oldLevel,
                              int const newLevel) {
  ImageSOA<uint16_t> outputImage(inputImage.r.size());
  for (size_t i = 0; i < inputImage.r.size(); ++i) {
    outputImage.r[i] =
        static_cast<uint16_t>((inputImage.r[i] * newLevel + oldLevel / 2)/ oldLevel);
    outputImage.g[i] =
        static_cast<uint16_t>((inputImage.g[i] * newLevel + oldLevel / 2) / oldLevel);
    outputImage.b[i] =
        static_cast<uint16_t>((inputImage.b[i] * newLevel + oldLevel / 2) / oldLevel);
  }
  return outputImage;
}

#endif  // MAXLEVEL_SOA_HPP