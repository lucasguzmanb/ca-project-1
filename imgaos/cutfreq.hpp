#ifndef CUTFREQ_AOS_HPP
#define CUTFREQ_AOS_HPP

#include "imageaos.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <queue>
#include <unordered_map>

template <typename T>
double euclideanDistance(Pixel<T> const & first, Pixel<T> const & second) {
  return std::sqrt(std::pow(first.r - second.r, 2) + std::pow(first.g - second.g, 2) +
                   std::pow(first.b - second.b, 2));
}

// Custom comparator for a pair of Pixel<T> and int
template <typename T>
bool comparePair(std::pair<Pixel<T>, int> const & pair1, std::pair<Pixel<T>, int> & pair2) {
  return pair1.second < pair2.second;
}

template <typename T>
void removeLFCaos(std::vector<Pixel<T>> & pixels, int n) {
  std::unordered_map<Pixel<T>, int, Pixel_map<T>> frequency;

  // Calculate frequency of each color
  for (auto const & pixel : pixels) { ++frequency[pixel]; }

  std::vector<std::pair<Pixel<T>, int>> frequencyVector(frequency.begin(), frequency.end());
  std::partial_sort(frequencyVector.begin(), frequencyVector.begin() + n, frequencyVector.end(),
                    comparePair<T>);

  // Divide colors in removed_pixels and remaining_colors
  std::vector<Pixel<T>> removed_pixels;
  std::vector<Pixel<T>> remainingColors;
  for (size_t i = 0; i < frequencyVector.size(); ++i) {
    if (i < static_cast<size_t>(n)) {
      removed_pixels.push_back(frequencyVector[i].first);
    } else {
      remainingColors.push_back(frequencyVector[i].first);
    }
  }

  std::unordered_map<Pixel<T>, Pixel<T>, Pixel_map<T>> replacementMap;

  for (auto const & pixel : removed_pixels) {
    double minDistance    = std::numeric_limits<double>::max();
    Pixel<T> closestColor = pixel;  // Start with the same color

    for (const auto& remainingColor : remainingColors) {
      double distanceSquared = euclideanDistanceSquared(pixel, remainingColor);
      if (distanceSquared < minDistance && pixel != remainingColor) {
        minDistance = distanceSquared;
        closestColor = remainingColor;
      }
    }
    replacementMap[pixel] = closestColor;
  }
  // Replace pixels in the original image
  for (auto & pixel : pixels) {
    auto newPixel = replacementMap.find(pixel);
    if (newPixel != replacementMap.end()) {
      pixel = newPixel->second;  // Update pixel color
    }
  }
}

#endif  // CUTFREQ_AOS_HPP