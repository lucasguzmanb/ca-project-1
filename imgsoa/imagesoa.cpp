/* Support for image processing with SOA */
#include "imagesoa.hpp"


template <typename T>
ImageSOA<T> binaryToSOA(std::ifstream & inputFile, int const width, int const height) {
  /*
   * read binary data from binary file, crete vector that stores colour values consequently
   * using technique of structure of arrays, create such structure,
   * that stores arrays for red, green and blue colour values of pixels, return this vector
   */
  std::vector<T> tempData = readRawData<T>(inputFile, width, height);
  ImageSOA<T> data(static_cast<std::size_t>(width * height));

  // Convert the raw data to SOA
  for (std::size_t i = 0, j = 0; i < data.r.size(); ++i, j += 3) {
      data.r[i] = tempData[j];
      data.g[i] = tempData[j + 1];
      data.b[i] = tempData[j + 2];
  }
  return data;
}

template <typename T>
void SOAToBinary(std::ofstream & outputFile, ImageSOA<T> const & data) {
  /*
   * read structure that stores vectors of pixels colour values(each for red, green, blue),
   * write data to binary file
   */
  // Convert the SOA data to raw data
  std::vector<T> tempData(data.r.size() * 3);

  for (std::size_t i = 0, j = 0; i < data.r.size(); ++i, j += 3) {
      tempData[j]     = data.r[i];
      tempData[j + 1] = data.g[i];
      tempData[j + 2] = data.b[i];
  }
  // Write the raw data to the output file
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputFile.write(reinterpret_cast<char const *>(tempData.data()),
                   static_cast<std::streamsize>(tempData.size() * sizeof(T)));
}

// explicitly tell the compiler to create template specialization of functions for different cases
template ImageSOA<uint8_t> binaryToSOA(std::ifstream & inputFile, int const width, int const height);
template ImageSOA<uint16_t> binaryToSOA(std::ifstream & inputFile, int const width, int const height);
template void SOAToBinary(std::ofstream & outputFile, ImageSOA<uint8_t> const & data);
template void SOAToBinary(std::ofstream & outputFile, ImageSOA<uint16_t> const & data);