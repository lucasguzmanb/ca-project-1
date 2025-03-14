#include "imageaos.hpp"

#include "common/progargs.hpp"

template <typename T>
std::vector<Pixel<T>> binaryToAOS(std::ifstream & inputFile, int const width, int const height) {
  /*
   * read binary data from binary file, crete vector that stores colour values consequently
   * using technique of array of structures, create such array,
   * that stores objects of pixels, return this vector
   */
  std::vector<T> tempData = readRawData<T>(inputFile, width, height);
  std::vector<Pixel<T>> data(static_cast<std::size_t>(width * height));

  // Convert the raw data to AOS
  for (std::size_t i = 0, j = 0; i < data.size(); ++i, j += 3) {
    data[i].r = tempData[j];
    data[i].g = tempData[j + 1];
    data[i].b = tempData[j + 2];
  }

  return data;
}

template <typename T>
void AOSToBinary(std::ofstream & outputFile, std::vector<Pixel<T>> const & data) {
  /*
   * read vector that stores objects of pixels, write data to binary file
   */
  // Convert the AOS data to raw data
  std::vector<T> tempData(data.size() * 3);

  for (std::size_t i = 0, j = 0; i < data.size(); ++i, j += 3) {
    tempData[j]     = data[i].r;
    tempData[j + 1] = data[i].g;
    tempData[j + 2] = data[i].b;
  }

  // Write the raw data to the binary file
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputFile.write(reinterpret_cast<char const *>(tempData.data()),
                   static_cast<std::streamsize>(tempData.size() * sizeof(T)));
}

template <typename T>
void AOSToBinary_(std::ofstream & outputFile, T const & data) {
  // Write the single instance of T in binary to the output file
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputFile.write(reinterpret_cast<char const *>(&data), sizeof(T));
}

// explicitly tell the compiler to create template specialization of functions for different cases
template std::vector<Pixel<uint8_t>> binaryToAOS(std::ifstream & inputFile, int width, int height);
template std::vector<Pixel<uint16_t>> binaryToAOS(std::ifstream & inputFile, int width, int height);
template void AOSToBinary(std::ofstream & outputFile, std::vector<Pixel<uint8_t>> const & data);
template void AOSToBinary(std::ofstream & outputFile, std::vector<Pixel<uint16_t>> const & data);
template void AOSToBinary_(std::ofstream & outputFile, uint8_t const & data);
template void AOSToBinary_(std::ofstream & outputFile, uint16_t const & data);
template void AOSToBinary_(std::ofstream & outputFile, uint32_t const & data);