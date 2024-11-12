#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <vector>
#include "imageaos.hpp"
#include <map>


template <typename T>
void compress(const std::vector<Pixel<T>> &inputPixels, std::ofstream &outputFile) {
    //int const maxColorValue = metadata.maxColorValue;
    std:: map<Pixel<T>, uint32_t> pixelMap = getColors(inputPixels);
    writeCompressedData(inputPixels,pixelMap, outputFile);
}


template <typename T>
std::map<Pixel<T>, uint32_t> getColors(const std::vector<Pixel<T>>& inputPixels) {
  std::map<Pixel<T>, uint32_t> pixelMap;
  for (const auto& pixel : inputPixels) {
    // Check if the color is in the map
    if (pixelMap.find(pixel) == pixelMap.end()) {
      // Assign a new index to this color
      auto index = static_cast<uint32_t>(pixelMap.size());
      pixelMap[pixel] = index;
    }
  }
  return pixelMap; // Return the pixel map
};



template <typename T>
void writeCompressedData(const std::vector<Pixel<T>>& inputPixels, std::map<Pixel<T>, uint32_t> &pixelMap, std::ofstream &outputFile) {
  // Write pixel data to outputFile
  const auto numcolors = pixelMap.size();
  std::string decimalString = std::to_string(numcolors);
  outputFile.write(decimalString.c_str(), static_cast<std::streamsize>(decimalString.size()));
  for (const auto& [pixel, index] : pixelMap) {
    const auto r = static_cast<T>(pixel.r);
    const auto g = static_cast<T>(pixel.g);
    const auto b = static_cast<T>(pixel.b);
    AOSToBinary_(outputFile, r);
    AOSToBinary_(outputFile, g);
    AOSToBinary_(outputFile, b);
  }

  constexpr int limitvalue = 255;
  constexpr int limitvalue2 = 65535;
  for (std::size_t i = 0; i < inputPixels.size(); i++) {
    auto it = pixelMap.find(inputPixels[i]);
    uint32_t index = it->second;
    if (pixelMap.size() <= limitvalue) {
      uint8_t index8 = static_cast<uint8_t>(index);
      AOSToBinary_(outputFile, index8);
    } else if (pixelMap.size() <= limitvalue2) {
      uint16_t index16 = static_cast<uint16_t>(index);
      AOSToBinary_(outputFile, index16);
    } else {
      AOSToBinary_(outputFile, index); // Write full 32-bit index
    }
    }
  }
#endif //COMPRESS_HPP