#include "maxlevel.hpp"

#include <vector>

template <typename T, typename U>
std::vector<Pixel<U>> maxlevel(std::vector<Pixel<T>> inputImage, int const oldLevel,
                               int const newLevel) {
  std::vector<Pixel<U>> outputImage(inputImage.size());
  double const oldLevelHalf    = oldLevel / 2.0;
  double const oldLevelInverse = 1.0 / oldLevel;

  for (size_t i = 0; i < inputImage.size(); ++i) {
    outputImage[i].r =
        static_cast<U>((inputImage[i].r * newLevel + oldLevelHalf) * oldLevelInverse);
    outputImage[i].g =
        static_cast<U>((inputImage[i].g * newLevel + oldLevelHalf) * oldLevelInverse);
    outputImage[i].b =
        static_cast<U>((inputImage[i].b * newLevel + oldLevelHalf) * oldLevelInverse);
  }

  return outputImage;
}

template std::vector<Pixel<uint8_t>> maxlevel(std::vector<Pixel<uint8_t>> inputImage,
                                              int oldLevel, int newLevel);

template std::vector<Pixel<uint8_t>> maxlevel(std::vector<Pixel<uint16_t>> inputImage,
                                              int oldLevel, int newLevel);

template std::vector<Pixel<uint16_t>> maxlevel(std::vector<Pixel<uint8_t>> inputImage,
                                               int oldLevel, int newLevel);

template std::vector<Pixel<uint16_t>> maxlevel(std::vector<Pixel<uint16_t>> inputImage,
                                               int oldLevel, int newLevel);