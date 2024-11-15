#ifndef COMPRESS_HPP
#define COMPRESS_HPP


#include "common/binaryio.hpp"
#include "imagesoa.hpp"
#include <map>

template <typename T>
void compress(const ImageSOA<T> &inputPixels, std::ofstream &outputFile, Metadata const &metadata) {
  std::map<std::tuple<T,T,T>, uint32_t> pixelMap = getColors(inputPixels);
  writeCompressedData(inputPixels, pixelMap, outputFile, metadata);
}

template <typename T>
std::map<std::tuple<T,T,T>, uint32_t> getColors(const ImageSOA<T> &inputPixels) {
  std::map<std::tuple<T,T,T>, uint32_t> pixelMap;
  for (std::size_t i = 0; i < inputPixels.size(); i++) {
    std::tuple<T, T, T> const pixel = inputPixels.getPixel(i);
    if (!pixelMap.contains(pixel)) {
      // Assign a new index to this color
      auto index = static_cast<uint32_t>(pixelMap.size());
      pixelMap[pixel] = index;
    }
  }
  return pixelMap;
}


template <typename T>
void writeCompressedData(const ImageSOA<T> &inputPixels, std::map<std::tuple<T,T,T>, uint32_t>&pixelMap, std::ofstream &outputFile, Metadata const &metadata) {
  // Write pixel data to outputFile
  const auto numcolors = pixelMap.size();
  writeMetadataCPPM(outputFile, metadata, static_cast<int>(numcolors));
  for (const auto& [pixel, index] : pixelMap) {
    const auto red= static_cast<T>(get<0>(pixel));
    const auto green= static_cast<T>(get<1>(pixel));
    const auto blue= static_cast<T>(get<2>(pixel));
    SOAToBinary_(outputFile, red);
    SOAToBinary_(outputFile, green);
    SOAToBinary_(outputFile, blue);
  }
  constexpr int limitvalue = 255;
  constexpr int limitvalue2 = 65535;
  for (std::size_t i = 0; i < inputPixels.size(); i++) {
    auto iter = pixelMap.find(inputPixels.getPixel(i));
    uint32_t const index = iter->second;
    if (pixelMap.size() <= limitvalue) {
      auto index8 = static_cast<uint8_t>(index);
      SOAToBinary_(outputFile, index8);
    } else if (pixelMap.size() <= limitvalue2) {
      auto index16 = static_cast<uint16_t>(index);
      SOAToBinary_(outputFile, index16);
    } else {
      SOAToBinary_(outputFile, index);
    }
  }
}

#endif //COMPRESS_HPP