#include "maxlevel.hpp"

#include <cstdint>
#include <variant>

template <typename T>
using ImageVariant = std::variant<std::vector<ImageSoA<uint8_t>>, std::vector<ImageSoA<uint16_t>>>;

template <typename T>
ImageSoA<T> maxlevel(ImageSoA<T> inputImage, int oldLevel, int newLevel) {
  ImageSoA<T> outputImage;

  // Check if we need to use 8-bit or 16-bit pixels for the output image
  if (newLevel <= 255) {
    outputImage.r = std::vector<uint8_t>(inputImage.r.size());
    outputImage.g = std::vector<uint8_t>(inputImage.g.size());
    outputImage.b = std::vector<uint8_t>(inputImage.b.size());
  } else {
    outputImage = std::vector<uint16_t>(inputImage.size());
    outputImage = std::vector<uint16_t>(inputImage.size());
    outputImage = std::vector<uint16_t>(inputImage.size());
  }

  // Make the intensity transformation with a formula that rounds the result to the nearest integer
  for (size_t i = 0; i < inputImage.r.size(); ++i) {
    outputImage.r[i] = (inputImage.r[i] * newLevel + oldLevel / 2) / oldLevel;
    outputImage.g[i] = (inputImage.g[i] * newLevel + oldLevel / 2) / oldLevel;
    outputImage.b[i] = (inputImage.b[i] * newLevel + oldLevel / 2) / oldLevel;
  }

  return outputImage;
}