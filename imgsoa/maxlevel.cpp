#include "maxlevel.hpp"


template <typename T, typename U>
ImageSOA<U> maxlevel(ImageSOA<T> inputImage, int const oldLevel, int const newLevel) {
  ImageSOA<U> outputImage(inputImage.r.size());
  double const transformation = newLevel * (1.0 / oldLevel);
  // Convert each pixel to the new level using a formula to round the result
  for (size_t i = 0; i < inputImage.r.size(); ++i) {
    outputImage.r[i] =
        static_cast<U>(inputImage.r[i] * transformation);
    outputImage.g[i] =
        static_cast<U>(inputImage.g[i] * transformation);
    outputImage.b[i] =
        static_cast<U>(inputImage.b[i] * transformation);
  }
  return outputImage;
}

template ImageSOA<uint8_t> maxlevel(ImageSOA<uint8_t> inputImage, int oldLevel, int newLevel);

template ImageSOA<uint8_t> maxlevel(ImageSOA<uint16_t> inputImage, int oldLevel, int newLevel);

template ImageSOA<uint16_t> maxlevel(ImageSOA<uint8_t> inputImage, int oldLevel, int newLevel);

template ImageSOA<uint16_t> maxlevel(ImageSOA<uint16_t> inputImage, int oldLevel, int newLevel);