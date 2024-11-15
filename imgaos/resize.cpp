#include "resize.hpp"

// Function for pixel interpolation
/*
   * perform linear interpolation and return its result
   * Linear interpolation formula:
   *      y = y1 + (x - x1) * (y2 - y1) / (x2 - x1),
   * perform linear interpolation with frac being quotient of x - x1 and x2 - x1
   * where xi - coordinates along certain axes and yi - color values
 */
template <typename T>
Pixel<T> interpolation(Pixel<T> c1, Pixel<T> c2, double frac) {
  Pixel<T> result;
  result.r = static_cast<T>(c1.r + ((c2.r - c1.r) * frac));
  result.g = static_cast<T>(c1.g + ((c2.g - c1.g) * frac));
  result.b = static_cast<T>(c1.b + ((c2.b - c1.b) * frac));
  return result;
}

/*
 * perform scaling of the size of the picture to a certain width and height
 * use linear interpolation for this
 * return vector storing pixels of new resized photo
 *
 * linear interpolation:
 * y = y1 + (x - x1) * (y2 - y1) / (x2 - x1),
 * where xi is a coordinates and yi is the colour at that point
 * compute coordinates of corresponding pixels at old photo to a (i, j) pixel in new one
 */

// compute exact (i, j) coordinate representation on the old grid
// and subtract by the lower coordinates to get x - x1 factor in expression
// also there is no need to divide by x2-x1, because it will 1 = x_h - x_l = y_h - y_l

template <typename T>
std::vector<Pixel<T> > resize(std::vector<Pixel<T> > pixels, Metadata const & metadata,
                             std::vector<int> const & size){

  int newWidth = size[0];
  int newHeight = size[1];
  std::vector<Pixel<T> > newPixels(static_cast<size_t>(newWidth * newHeight));
  Pixel<T> color1{};
  Pixel<T> color2{};
  for (int i = 0; i < newHeight; i++) {
    for (int j = 0; j < newWidth; j++) {
      // compute coordinates of corresponding pixels at old photo to a (i, j) pixel in new one
      int const x_l = j * metadata.width / newWidth;
      int const y_l = i * metadata.height / newHeight;
      int const x_h = std::min(x_l + 1, metadata.width - 1);   // To avoid out-of-bounds access
      int const y_h = std::min(y_l + 1, metadata.height - 1);  // To avoid out-of-bounds access
      double const x_diff = (j * static_cast<double>(metadata.width) / newWidth) - x_l;
      double const y_diff = (i * static_cast<double>(metadata.height) / newHeight) - y_l;
      // interpolate between (x_l, y_l) and (x_h, y_l)
      color1 = interpolation<T>(
          pixels[(static_cast<size_t>(y_l) * static_cast<size_t>(metadata.width)) +
                 static_cast<size_t>(x_l)],
          pixels[(static_cast<size_t>(y_l) * static_cast<size_t>(metadata.width)) +
                 static_cast<size_t>(x_h)],
          x_diff);
      // interpolate between (x_l, y_l) and (x_h, y_l)
      color2 = interpolation<T>(
          pixels[(static_cast<size_t>(y_h) * static_cast<size_t>(metadata.width)) +
                 static_cast<size_t>(x_l)],
          pixels[(static_cast<size_t>(y_h) * static_cast<size_t>(metadata.width)) +
                 static_cast<size_t>(x_h)],
          x_diff);
      // interpolation along the y-axis
      newPixels[(static_cast<size_t>(i) * static_cast<size_t>(newWidth)) +
                static_cast<size_t>(j)] = interpolation<T>(color1, color2, y_diff);
    }
  }
  return newPixels;
}

// explicitly instantiate the required template specializations
template std::vector<Pixel<uint8_t>> resize<uint8_t>(
    std::vector<Pixel<uint8_t>> pixels,
    Metadata const & metadata,
    std::vector<int> const & size);
template std::vector<Pixel<uint16_t>> resize<uint16_t>(
    std::vector<Pixel<uint16_t>> pixels,
    Metadata const & metadata,
    std::vector<int> const & size);