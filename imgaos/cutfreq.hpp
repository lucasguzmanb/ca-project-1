#ifndef CUTFREQ_AOS_HPP
#define CUTFREQ_AOS_HPP

#include "imageaos.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <unordered_map>
#include <queue>


template <typename T>
double euclideanDistance(Pixel<T> const & first, Pixel<T> const & second) {
  return std::sqrt(std::pow(first.r - second.r, 2) + std::pow(first.g - second.g, 2) +
                   std::pow(first.b - second.b, 2));
}

template <typename T>
void removeLFCaos(std::vector<Pixel<T>> & pixels, int n) {
  std::unordered_map<Pixel<T>, int, Pixel_map<T>> frequency;

  // Calculate frequency of each color
  for (const auto & pixel : pixels) {
    ++frequency[pixel];
  }

  // Use partial sorting to get the n least frequent colors
  std::vector<std::pair<int, Pixel<T>>> frequencyVec;
  for (const auto& entry : frequency) {
    frequencyVec.emplace_back(entry.second, entry.first);
  }

  // Sort only up to the n least frequent colors
  std::partial_sort(frequencyVec.begin(), frequencyVec.begin() + n, frequencyVec.end());

  // Collect least frequent colors and prepare remaining colors
  std::vector<Pixel<T>> removed_pixels, remainingColors;
  for (size_t i = 0; i < frequencyVec.size(); ++i) {
    if (i < static_cast<size_t>(n)) {
      removed_pixels.push_back(frequencyVec[i].second);
    } else {
      remainingColors.push_back(frequencyVec[i].second);
    }
  }


  // Create a replacement map for the least frequent colors
  std::unordered_map<Pixel<T>, Pixel<T>, Pixel_map<T>> replacementMap;

  // Find nearest color for each removed pixel
  for (const auto& pixel : removed_pixels) {
    double minDistance = std::numeric_limits<double>::max();
    Pixel<T> closestColor = pixel;  // Default to itself in case of no better match

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
  for (auto &pixel : pixels) {
    auto it = replacementMap.find(pixel);
    if (it != replacementMap.end()) {
      pixel = it->second;  // Update pixel color

    }
  }
}



#endif  // CUTFREQ_AOS_HPP