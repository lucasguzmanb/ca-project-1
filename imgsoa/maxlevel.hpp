#ifndef MAXLEVEL_SOA_HPP
#define MAXLEVEL_SOA_HPP
#include "imagesoa.hpp"

#include <cmath>
#include <cstdint>
#include <variant>

template <typename T>
std::variant<ImageSOA<uint8_t>, ImageSOA<uint16_t>> maxlevel(ImageSOA<T> inputImage, int const oldLevel, int const newLevel) {
  float const transformation = static_cast<float>(newLevel) / static_cast<float>(oldLevel);

  // input image is <T>, but output image is either uint8_t or uint16_t
  if (constexpr int THRESHOLD = 255; newLevel <= THRESHOLD) {
    return maxlevel8<T>(inputImage, transformation);
  }
  return maxlevel16<T>(inputImage, transformation);
}

template <typename T>
ImageSOA<uint8_t> maxlevel8(ImageSOA<T> const & inputImage, float const transformation) {
  ImageSOA<uint8_t> outputImage;
  outputImage.r.resize(inputImage.r.size());
  outputImage.g.resize(inputImage.g.size());
  outputImage.b.resize(inputImage.b.size());
  for (size_t i = 0; i < inputImage.r.size(); ++i) {
    outputImage.r[i] = static_cast<uint8_t>(std::round(inputImage.r[i] * transformation));
    outputImage.g[i] = static_cast<uint8_t>(std::round(inputImage.g[i] * transformation));
    outputImage.b[i] = static_cast<uint8_t>(std::round(inputImage.b[i] * transformation));
  }
  return outputImage;
}

template <typename T>
ImageSOA<uint16_t> maxlevel16(ImageSOA<T> const & inputImage, float const transformation) {
  ImageSOA<uint16_t> outputImage;
  outputImage.r.resize(inputImage.r.size());
  outputImage.g.resize(inputImage.g.size());
  outputImage.b.resize(inputImage.b.size());
  for (size_t i = 0; i < inputImage.r.size(); ++i) {
    outputImage.r[i] = static_cast<uint16_t>(std::round(inputImage.r[i] * transformation));
    outputImage.g[i] = static_cast<uint16_t>(std::round(inputImage.g[i] * transformation));
    outputImage.b[i] = static_cast<uint16_t>(std::round(inputImage.b[i] * transformation));
  }
  return outputImage;
}

#endif  // MAXLEVEL_SOA_HPP