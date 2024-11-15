#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <vector>
#include "imageaos.hpp"
#include "common/binaryio.hpp"
#include <map>


template <typename T>
void compress(const std::vector<Pixel<T>> &inputPixels, std::ofstream &outputFile, Metadata const &metadata) {
    //int const maxColorValue = metadata.maxColorValue;
    std:: map<Pixel<T>, uint32_t> pixelMap = getColors(inputPixels);
    writeCompressedData(inputPixels,pixelMap, outputFile, metadata);
}


template <typename T>
std::map<Pixel<T>, uint32_t> getColors(const std::vector<Pixel<T>>& inputPixels) {
  std::map<Pixel<T>, uint32_t> pixelMap;
  for (const auto& pixel : inputPixels) {
    // Check if the color is in the map
    if (!pixelMap.contains(pixel)) {
      // Assign a new index to this color
      auto index = static_cast<uint32_t>(pixelMap.size());
      pixelMap[pixel] = index;
    }
  }
  return pixelMap; // Return the pixel map
};



template <typename T>
void writeCompressedData(const std::vector<Pixel<T>>& inputPixels, std::map<Pixel<T>, uint32_t> &pixelMap, std::ofstream &outputFile, Metadata const &metadata) {
  // Write pixel data to outputFile
  const auto numcolors = pixelMap.size();
  //std::string decimalString = std::to_string(numcolors);
  writeMetadataCPPM(outputFile, metadata, static_cast<int>(numcolors));
  //outputFile.write(decimalString.c_str(), static_cast<std::streamsize>(decimalString.size()));
  for (const auto& [pixel, index] : pixelMap) {
    const auto red = static_cast<T>(pixel.r);
    const auto green = static_cast<T>(pixel.g);
    const auto blue = static_cast<T>(pixel.b);
    AOSToBinary_(outputFile, red);
    AOSToBinary_(outputFile, green);
    AOSToBinary_(outputFile, blue);
  }

  constexpr int limitvalue = 255;
  constexpr int limitvalue2 = 65535;
  for (std::size_t i = 0; i < inputPixels.size(); i++) {
    auto iter = pixelMap.find(inputPixels[i]);
    uint32_t index = iter->second;
    if (pixelMap.size() <= limitvalue) {
      auto index8 = static_cast<uint8_t>(index);
      AOSToBinary_(outputFile, index8);
    } else if (pixelMap.size() <= limitvalue2) {
      auto index16 = static_cast<uint16_t>(index);
      AOSToBinary_(outputFile, index16);
    } else {
      AOSToBinary_(outputFile, index);
    }
    }
  }
#endif //COMPRESS_HPP