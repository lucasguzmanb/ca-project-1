/* Support for image processing with SOA */
#include "imagesoa.hpp"


template <typename T>
ImageSOA<T> binaryToSOA(std::ifstream & inputFile, int const width, int const height) {

  std::vector<T> tempData = readRawData<T>(inputFile, width, height);
  ImageSOA<T> data(static_cast<std::size_t>(width * height));

  // Convert the raw data to SOA
  for (std::size_t i = 0, j = 0; i < data.r.size(); ++i, j += 3) {
    if (sizeof(T) == 2){
      data.r[i] = static_cast<T>((tempData[j] << 8) | (tempData[j] >> 8));
      data.g[i] = static_cast<T>((tempData[j+1] << 8) | (tempData[j+1] >> 8));
      data.b[i] = static_cast<T>((tempData[j+2] << 8) | (tempData[j+2] >> 8));
    }
    else {
      data.r[i] = tempData[j];
      data.g[i] = tempData[j + 1];
      data.b[i] = tempData[j + 2];
    }
  }

  return data;
}

template <typename T>
void SOAToBinary(std::ofstream & outputFile, ImageSOA<T> const & data) {
  // Convert the SOA data to raw data
  std::vector<T> tempData(data.r.size() * 3);

  for (std::size_t i = 0, j = 0; i < data.r.size(); ++i, j += 3) {
    if (sizeof(T) == 2){
      tempData[j]     = static_cast<T>((data.r[i] << 8) | (data.r[i] >> 8));
      tempData[j + 1] = static_cast<T>((data.g[i] << 8) | (data.g[i] >> 8));
      tempData[j + 2] = static_cast<T>((data.b[i] << 8) | (data.b[i] >> 8));
    }
    else {
      tempData[j]     = data.r[i];
      tempData[j + 1] = data.g[i];
      tempData[j + 2] = data.b[i];
    }
  }

  // Write the raw data to the output file
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputFile.write(reinterpret_cast<char const *>(tempData.data()),
                   static_cast<std::streamsize>(tempData.size() * sizeof(T)));
}


template ImageSOA<uint8_t> binaryToSOA(std::ifstream & inputFile, int const width, int const height);
template ImageSOA<uint16_t> binaryToSOA(std::ifstream & inputFile, int const width, int const height);
template void SOAToBinary(std::ofstream & outputFile, ImageSOA<uint8_t> const & data);
template void SOAToBinary(std::ofstream & outputFile, ImageSOA<uint16_t> const & data);
