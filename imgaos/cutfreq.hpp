#ifndef CUTFREQ_HPP
#define CUTFREQ_HPP
#include "imageaos.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <unordered_map>

template <typename T>
struct Pixel_map {
    std::size_t operator()(Pixel<T> const & color) const {
      return std::hash<T>{}(color.r) ^ std::hash<T>{}(color.g) ^ std::hash<T>{}(color.b);
    }
};

template <typename T>
double euclideanDistance(Pixel<T> const & first, Pixel<T> const & second) {
  return std::sqrt(std::pow(first.r - second.r, 2) + std::pow(first.g - second.g, 2) +
                   std::pow(first.b - second.b, 2));
}

template <typename T>
void removeLFCaos(std::vector<Pixel<T>> & pixels, int n) {
  std::unordered_map<Pixel<T>, int, Pixel_map<T>> frequency;

  // Calculate frequency of each color
  for (auto const & pixel : pixels) { frequency[pixel]++; }

  // Create a vector of color frequencies
  std::vector<std::pair<Pixel<T>, int>> colorFreq(frequency.begin(), frequency.end());
  std::sort(colorFreq.begin(), colorFreq.end(), [](auto const & a, auto const & b) {
    if (a.second != b.second) {
      return a.second < b.second;  // Sort by frequency
    }
    return std::tie(b.first.b, b.first.g, b.first.r) <
           std::tie(a.first.b, a.first.g, a.first.r);  // Tie-breaking by b, g, r
  });

  // Collect least frequent colors
  std::vector<Pixel<T>> removed_pixels;
  for (size_t i = 0; i < static_cast<size_t>(n) && i < colorFreq.size(); i++) {
    removed_pixels.push_back(colorFreq[i].first);
  }

  // Map to replace the least frequent colors with the closest colors
  std::unordered_map<Pixel<T>, Pixel<T>, Pixel_map<T>> replacementMap;
  for (auto const & pixel : removed_pixels) {
    double minDistance    = std::numeric_limits<double>::max();
    Pixel<T> closestColor = pixel;

    for (auto const & [remainingColor, freq] : colorFreq) {
      if (std::find(removed_pixels.begin(), removed_pixels.end(), remainingColor) ==
          removed_pixels.end()) {
        double distance = euclideanDistance(pixel, remainingColor);
        if (distance < minDistance) {
          minDistance  = distance;
          closestColor = remainingColor;
        }
      }
    }
    replacementMap[pixel] = closestColor;
  }

  // Replace pixels in the original image
  for (auto & pixel : pixels) {
    auto it = replacementMap.find(pixel);
    if (it != replacementMap.end()) {
      pixel = it->second;  // Update pixel color
    }
  }
}

#endif  // CUTFREQ_HPP