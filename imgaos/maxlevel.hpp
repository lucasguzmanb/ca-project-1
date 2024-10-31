#ifndef MAXLEVEL_AOS_HPP
#define MAXLEVEL_AOS_HPP

#include "imageaos.hpp"

#include <cstdint>
#include <variant>
#include <vector>

template <typename T, typename U>
std::vector<Pixel<U>> maxlevel(std::vector<Pixel<T>> inputImage, int const oldLevel, int const newLevel) {
    std::vector<Pixel<U>> outputImage(inputImage.size());
        for (size_t i = 0; i < inputImage.size(); ++i) {
                outputImage[i].r = static_cast<U>((inputImage[i].r * newLevel + oldLevel / 2) / oldLevel);
                outputImage[i].g = static_cast<U>((inputImage[i].g * newLevel + oldLevel / 2) / oldLevel);
                outputImage[i].b = static_cast<U>((inputImage[i].b * newLevel + oldLevel / 2) / oldLevel);
        }
  return outputImage;
}

template <typename T>
std::vector<Pixel<uint8_t>> maxlevel8(std::vector<Pixel<T>> const & inputImage, int const oldLevel,
                                      int const newLevel) {
  std::vector<Pixel<uint8_t>> outputImage(inputImage.size());
  for (size_t i = 0; i < inputImage.size(); ++i) {
    outputImage[i].r =
        static_cast<uint8_t>((inputImage[i].r * newLevel + oldLevel / 2) / oldLevel);
    outputImage[i].g =
        static_cast<uint8_t>((inputImage[i].g * newLevel + oldLevel / 2) / oldLevel);
    outputImage[i].b =
        static_cast<uint8_t>((inputImage[i].b * newLevel + oldLevel / 2) / oldLevel);
  }
  return outputImage;
}

template <typename T>
std::vector<Pixel<uint16_t>> maxlevel16(std::vector<Pixel<T>> const & inputImage,
                                        int const oldLevel, int const newLevel) {
  std::vector<Pixel<uint16_t>> outputImage(inputImage.size());
  for (size_t i = 0; i < inputImage.size(); ++i) {
    outputImage[i].r =
        static_cast<uint16_t>((inputImage[i].r * newLevel + oldLevel / 2) / oldLevel);
    outputImage[i].g =
        static_cast<uint16_t>((inputImage[i].g * newLevel + oldLevel / 2) / oldLevel);
    outputImage[i].b =
        static_cast<uint16_t>((inputImage[i].b * newLevel + oldLevel / 2) / oldLevel);
  }
  return outputImage;
}

#endif  // MAXLEVEL_AOS_HPP