/* Support for image processing with SOA */
#include "imagesoa.hpp"

template <typename T>
ImageSOA<T> binaryToSOA(const std::vector<T>& rawData, int const width, int const height) {
  std::size_t dataSize = static_cast<std::size_t>(width) * height;
  ImageSOA<T> data;
  data.r.resize(dataSize);
  data.g.resize(dataSize);
  data.b.resize(dataSize);
  for (std::size_t i = 0; i < dataSize; ++i) {
    data.r[i] = rawData[i * 3];
    data.g[i] = rawData[i * 3 + 1];
    data.b[i] = rawData[i * 3 + 2];
  }
  return data;
}