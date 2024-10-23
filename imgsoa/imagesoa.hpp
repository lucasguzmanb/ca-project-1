/* Support for image processing with SOA (headers) */
#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP
#include <vector>

template <typename T>
struct ImageSOA {
  std::vector<T> r, g, b;
};

template <typename T>
ImageSOA<T> binaryToSOA(const std::vector<T>& rawData, int width, int height);

#endif // IMAGESOA_HPP