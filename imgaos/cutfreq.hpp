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

  // Use a priority queue to find the least frequent colors
  using FrequencyPair = std::pair<int, Pixel<T>>;
  auto compare = [](const FrequencyPair &a, const FrequencyPair &b) {
    return a.first > b.first;  // Min-heap based on frequency
  };
  std::priority_queue<FrequencyPair, std::vector<FrequencyPair>, decltype(compare)> minHeap(compare);

  for (const auto &entry : frequency) {
    minHeap.emplace(entry.second, entry.first);
  }

  // Collect least frequent colors
  std::vector<Pixel<T>> removed_pixels;
  while (removed_pixels.size() < static_cast<size_t>(n) && !minHeap.empty()) {
    removed_pixels.push_back(minHeap.top().second);
    minHeap.pop();
  }

  // Create a replacement map for the least frequent colors
  std::unordered_map<Pixel<T>, Pixel<T>, Pixel_map<T>> replacementMap;
  std::vector<Pixel<T>> remainingColors;

  for (const auto &entry : frequency) {
    if (std::find(removed_pixels.begin(), removed_pixels.end(), entry.first) == removed_pixels.end()) {
      remainingColors.push_back(entry.first);
    }
  }

  for (const auto &pixel : removed_pixels) {
    double minDistance = std::numeric_limits<double>::max();
    Pixel<T> closestColor = pixel;  // Start with the same color

    for (const auto &remainingColor : remainingColors) {
      double distance = euclideanDistance(pixel, remainingColor);
      if (distance < minDistance && pixel != remainingColor) {
        minDistance = distance;
        closestColor = remainingColor;

      }
    }

    if (closestColor != pixel) {
      // Ensure it's different before adding
      replacementMap[pixel] = closestColor;
    }
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