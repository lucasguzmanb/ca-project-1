#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "imageaos.hpp"
#include <map>


template <typename T>
std::vector<Pixel<T>> compress(const std::vector<Pixel<T>> inputPixels, Metadata metadata) {
    //initialize values
    int const maxColorValue = metadata.maxColorValue;
    std:: map<Pixel<T>, uint16_t> pixelMap = getColors(inputPixels);
    std::vector<Pixel<T>> outputPixels = writeCompressedData(pixelMap, maxColorValue);
    return outputPixels;

}


template <typename T>
std::map<Pixel<T>, uint16_t> getColors(const std::vector<Pixel<T>>& inputPixels) {
  std::map<Pixel<T>, uint16_t> pixelMap;
  for (const auto& pixel : inputPixels) {
    // Check if the color is in the map
    if (pixelMap.find(pixel) == pixelMap.end()) {
      // Assign a new index to this color
      uint16_t index = static_cast<uint16_t>(pixelMap.size());
      pixelMap[pixel] = index;
    }
  }
  return pixelMap; // Return the pixel map
}



template <typename T>
std::vector<Pixel<T>> writeCompressedData(std::map<Pixel<T>, uint16_t> &pixelMap, int maxColorValue){
  constexpr int limitvalue = 255;
  constexpr int limitvalue2 = 65535;
  std::vector<Pixel<T>> outputPixels;
  // Write pixel data to outputPixels vector
  for (const auto& [pixel, index] : pixelMap) {
    // Store the pixel in the vector
    if (maxColorValue <= limitvalue) {
      // For 8-bit color depth
      uint8_t r = static_cast<uint8_t>(pixel.r);
      uint8_t g = static_cast<uint8_t>(pixel.g);
      uint8_t b = static_cast<uint8_t>(pixel.b);
      outputPixels.push_back(Pixel<T>{r, g, b}); // Add the pixel to the vector
    } else {
      // For higher color depths
      outputPixels.push_back(pixel); // Add the pixel to the vector directly
    }
  }
  // Write indices for the pixel data
  for (const auto& [pixel, index] : pixelMap) {
    if (pixelMap.size() <= limitvalue) {
      uint8_t index8 = static_cast<uint8_t>(index);
      outputPixels.push_back(Pixel<T>{index8, 0, 0}); // Store index as a pixel (dummy values for g and b)
    } else if (pixelMap.size() <= limitvalue2) {
      uint16_t index16 = static_cast<uint16_t>(index);
      outputPixels.push_back(Pixel<T>{index16, 0, 0}); // Store index as a pixel (dummy values for g and b)
    } else {
      uint32_t index32 = static_cast<uint32_t>(index);
      outputPixels.push_back(Pixel<T>{index32, 0, 0}); // Store index as a pixel (dummy values for g and b)
    }
  }

  return outputPixels; // Return the vector of output pixels


};

#endif //COMPRESS_HPP