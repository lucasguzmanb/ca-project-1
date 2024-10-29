#ifndef MAXLEVEL_AOS_HPP
#define MAXLEVEL_AOS_HPP

#include "imageaos.hpp"

#include <cmath>
#include <cstdint>
#include <variant>
#include <vector>

template <typename T>
std::variant<std::vector<Pixel<uint8_t>>, std::vector<Pixel<uint16_t>>>
    maxlevel(std::vector<Pixel<T>> inputImage, int const oldLevel, int const newLevel) {

  const float transformation = static_cast<float>(newLevel) / static_cast<float>(oldLevel);

  if (constexpr int THRESHOLD = 255; newLevel <= THRESHOLD) { return maxlevel8<T>(inputImage, transformation); }
  return maxlevel16<T>(inputImage, transformation);
}

template <typename T>
std::vector<Pixel<uint8_t>> maxlevel8(std::vector<Pixel<T>> const & inputImage,
                                      float const transformation) {
  std::vector<Pixel<uint8_t>> outputImage(inputImage.size());
  for (size_t i = 0; i < inputImage.size(); ++i) {
    outputImage[i].r = static_cast<uint8_t>(std::round(inputImage[i].r * transformation));
    outputImage[i].g = static_cast<uint8_t>(std::round(inputImage[i].g * transformation));
    outputImage[i].b = static_cast<uint8_t>(std::round(inputImage[i].b * transformation));
  }
  return outputImage;
}

template <typename T>
std::vector<Pixel<uint16_t>> maxlevel16(std::vector<Pixel<T>> const & inputImage,
                                        float const transformation) {
  std::vector<Pixel<uint16_t>> outputImage(inputImage.size());
  for (size_t i = 0; i < inputImage.size(); ++i) {
    outputImage[i].r = static_cast<uint16_t>(std::round(inputImage[i].r * transformation));
    outputImage[i].g = static_cast<uint16_t>(std::round(inputImage[i].g * transformation));
    outputImage[i].b = static_cast<uint16_t>(std::round(inputImage[i].b * transformation));
  }
  return outputImage;
}
#endif  // MAXLEVEL_AOS_HPP