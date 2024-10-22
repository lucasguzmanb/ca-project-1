#include "maxlevel.hpp"

#include "imageaos.hpp"

#include <cstdint>
#include <variant>
#include <vector>


template <typename T>
using PixelVariant = std::variant<std::vector<Pixel<uint8_t>>, std::vector<Pixel<uint16_t>>>;


template <typename T>
PixelVariant<T> maxlevel(std::vector<Pixel<T>> inputImage, int oldLevel, int newLevel) {
  PixelVariant outputImage;

  // Check if we need to use 8-bit or 16-bit pixels for the output image
  if (newLevel <= 255) {
    outputImage = std::vector<Pixel<uint8_t>>(inputImage.size());
  } else {
    outputImage = std::vector<Pixel<uint16_t>>(inputImage.size());
  }

  // Make the intensity transformation with a formula that rounds the result to the nearest integer
  for (size_t i = 0; i < inputImage.size(); ++i) {
    outputImage[i] = (inputImage[i] * newLevel + oldLevel / 2) / oldLevel;
  }


  return outputImage;
}