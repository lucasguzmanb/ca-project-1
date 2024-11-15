#include "resize.hpp"

// Function for pixel interpolation
template <typename T>
T interpolate(T color1, T color2, double frac) {
  /*
   * perform linear interpolation and return its result
   * Linear interpolation formula:
   *      y = y1 + (x - x1) * (y2 - y1) / (x2 - x1),
   * perform linear interpolation with frac being quotient of x - x1 and x2 - x1
   * where xi - coordinates along certain axes and yi - color values
   */
  return static_cast<T> (color1 + ((color2 - color1) * frac));
}
template <typename T>
T interpolation(std::vector<T> &colorPixels, coordinates const coordinate,
                   std::vector<int> const & widths, std::vector<double> const & diff){
  // {j, i, x_l, y_l, x_h, y_h};
  T color1 = interpolate<T>(
      colorPixels[(static_cast<size_t>(coordinate.y_l) * static_cast<size_t>(widths[0])) +
               static_cast<size_t>(coordinate.x_l)],
      colorPixels[(static_cast<size_t>(coordinate.y_l) * static_cast<size_t>(widths[0])) +
               static_cast<size_t>(coordinate.x_h)],
      diff[0]);
  T color2 = interpolate<T>(
      colorPixels[(static_cast<size_t>(coordinate.y_h) * static_cast<size_t>(widths[0])) +
               static_cast<size_t>(coordinate.x_l)],
      colorPixels[(static_cast<size_t>(coordinate.y_h) * static_cast<size_t>(widths[0])) +
               static_cast<size_t>(coordinate.x_h)],
      diff[0]);
  return interpolate<T>(color1, color2, diff[1]);
}

/*
   * perform scaling of the size of the picture to a certain width and height
   * use linear interpolation for this
   * return structure storing vectors of pixels colour values of new resized photo
 */
template <typename T>
ImageSOA<T> resize(ImageSOA<T> pixels, Metadata const & metadata, std::vector<int> const & size){
  int const newWidth = size[0];  // Define the new dimensions
  int const newHeight = size[1];
  ImageSOA<T> newPixels;
  newPixels.resize(static_cast<size_t>(newWidth) * static_cast<size_t>(newHeight));
  for (int i = 0; i < newHeight; i++) {
    for (int j = 0; j < newWidth; j++) {
      int const x_l = j * metadata.width / newWidth;
      int const y_l = i * metadata.height / newHeight;
      int const x_h = std::min(x_l + 1, metadata.width - 1);   // To avoid out-of-bounds access
      int const y_h = std::min(y_l + 1, metadata.height - 1);  // To avoid out-of-bounds access
      double const x_diff = (j * static_cast<double>(metadata.width) / newWidth) - x_l;
      double const y_diff = (i * static_cast<double>(metadata.height) / newHeight) - y_l;
      coordinates const coordinate = {.j=j, .i=i, .x_l=x_l, .y_l=y_l, .x_h=x_h, .y_h=y_h};
      std::vector<int> const widths = {metadata.width, newWidth};
      std::vector<double> const diff = {x_diff, y_diff};
      // compute new red value
      newPixels.r[(static_cast<size_t>(i) * static_cast<size_t>(newWidth)) + static_cast<size_t>(j)]
          = interpolation<T>(pixels.r, coordinate, widths, diff);
      // compute new green value
      newPixels.b[(static_cast<size_t>(i) * static_cast<size_t>(newWidth)) + static_cast<size_t>(j)]
          = interpolation<T>(pixels.b, coordinate, widths, diff);
      // compute new blue value
      newPixels.g[(static_cast<size_t>(i) * static_cast<size_t>(newWidth)) + static_cast<size_t>(j)]
          = interpolation<T>(pixels.g, coordinate, widths, diff);
    }
  }
  return newPixels;
}


// explicitly instantiate the required template specializations
template ImageSOA<uint8_t> resize(ImageSOA<uint8_t> pixels, Metadata const & metadata,
                                  std::vector<int> const & size);
template ImageSOA<uint16_t> resize(ImageSOA<uint16_t> pixels, Metadata const & metadata,
                                   std::vector<int> const & size);