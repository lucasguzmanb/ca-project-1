#include "resize.hpp"

// Function for pixel interpolation
template <typename T>
T interpolation(T color1, T color2, double frac) {
  /*
   * perform linear interpolation and return its result
   * Linear interpolation formula:
   *      y = y1 + (x - x1) * (y2 - y1) / (x2 - x1),
   * perform linear interpolation with frac being quotient of x - x1 and x2 - x1
   * where xi - coordinates along certain axes and yi - color values
   */
  return static_cast<T> (color1 + ((color2 - color1) * frac));
}

/*
   * perform scaling of the size of the picture to a certain width and height
   * use linear interpolation for this
   * return structure storing vectors of pixels colour values of new resized photo
 */
template <typename T>
ImageSOA<T> resize(ImageSOA<T> pixels, Metadata metadata, std::vector<int> const & size){
  int const newWidth = size[0];  // Define the new dimensions
  int const newHeight = size[1];
  ImageSOA<T> newPixels;
  newPixels.resize(static_cast<size_t>(newWidth) * static_cast<size_t>(newHeight));
  T color1;
  T color2;
  for (int i = 0; i < newHeight; i++) {
    for (int j = 0; j < newWidth; j++) {
      int const x_l = j * metadata.width / newWidth;
      int const y_l = i * metadata.height / newHeight;
      int const x_h = std::min(x_l + 1, metadata.width - 1);   // To avoid out-of-bounds access
      int const y_h = std::min(y_l + 1, metadata.height - 1);  // To avoid out-of-bounds access
      double const x_diff = (j * static_cast<double>(metadata.width) / newWidth) - x_l;
      double const y_diff = (i * static_cast<double>(metadata.height) / newHeight) - y_l;
      // compute new red value
      color1 = interpolation<T>(
          pixels.r[(static_cast<size_t>(y_l) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_l)],
          pixels.r[(static_cast<size_t>(y_l) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_h)],
          x_diff);
      color2 = interpolation<T>(
          pixels.r[(static_cast<size_t>(y_h) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_l)],
          pixels.r[(static_cast<size_t>(y_h) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_h)],
          x_diff);
      newPixels.r[(static_cast<size_t>(i) * static_cast<size_t>(newWidth)) + static_cast<size_t>(j)]
          = interpolation<T>(color1, color2, y_diff);
      // compute new green value
      color1 = interpolation<T>(
          pixels.g[(static_cast<size_t>(y_l) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_l)],
          pixels.g[(static_cast<size_t>(y_l) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_h)],
          x_diff);
      color2 = interpolation<T>(
          pixels.g[(static_cast<size_t>(y_h) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_l)],
          pixels.g[(static_cast<size_t>(y_h) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_h)],
          x_diff);
      newPixels.g[(static_cast<size_t>(i) * static_cast<size_t>(newWidth)) + static_cast<size_t>(j)]
          = interpolation<T>(color1, color2, y_diff);
      // compute new blue value
      color1 = interpolation<T>(
          pixels.b[(static_cast<size_t>(y_l) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_l)],
          pixels.b[(static_cast<size_t>(y_l) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_h)],
          x_diff);
      color2 = interpolation<T>(
          pixels.b[(static_cast<size_t>(y_h) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_l)],
          pixels.b[(static_cast<size_t>(y_h) * static_cast<size_t>(metadata.width)) +
                   static_cast<size_t>(x_h)],
          x_diff);
      newPixels.b[(static_cast<size_t>(i) * static_cast<size_t>(newWidth)) + static_cast<size_t>(j)]
          = interpolation<T>(color1, color2, y_diff);
    }
  }
  return newPixels;
}


// explicitly instantiate the required template specializations
template ImageSOA<uint8_t> resize(ImageSOA<uint8_t> pixels, Metadata metadata,
                                  std::vector<int> const & size);
template ImageSOA<uint16_t> resize(ImageSOA<uint16_t> pixels, Metadata metadata,
                                   std::vector<int> const & size);