#include "cutfreq.hpp"

#include <cmath>
#include <cstddef>
#include <functional>

struct RGB {
  int r,g,b;

  bool operator==(const RGB &other) const {
    return std::tie(r, g, b) == std::tie(other.r, other.g, other.b);
  }
};
struct RGB_map {
  std::size_t operator()(const RGB& color) const {
    return std::hash<int>{}(color.r) + color.g + color.b;
  }
};
double euclideanDistance(const RGB& first, const RGB& second) {
  return std::sqrt(std::pow(first.r - second.r,2) + std::pow(first.g - second.g,2) + std::pow(first.b - second.b,2));
}
void removeLFCaos(std::vector<RGB>& pixels) {
  std::unordered_map<RGB, int, RGB_map> frequency;
  for (const auto& pixel : pixels) {
    frequency[pixel]++;
  }
  std::vector<std::pair<RGB, int>> colorFreq(frequency.begin(), frequency.end());
  std::sort(colorFreq.begin(), colorFreq.end(), [](const auto& a, const auto& b) {
    if (a.second != b.second) return a.second < b.second; //Sort by frequency
    return std::tie(b.first.b, b.first.g, b.first.r) < std::tie(a.first.b, a.first.g, a.first.r);
  });
  std::vector<RGB> removed_pixels;
  for (int i = 0; i < n && i < colorFreq.size(); i++) {
    removed_pixels.push_back(colorFreq[i].first);
  }
  std::unordered_map<RGB, int, RGB_map> replacementMap;
  for (const auto& pixel : removed_pixels) {
    double minDistance = std::numeric_limits<double>::max();
    RGB closestColor = pixel;
    for (const auto& [remainingColor, freq] : colorFreq) {
      if (std::find(removed_pixels.begin(), removed_pixels.end(), remainingColor) == removed_pixels.end()) {
        double distance = euclideanDistance(pixel, remainingColor);
        if (distance < minDistance) {
          minDistance = distance;
          closestColor = remainingColor;
        }
      }

    }
    replacementMap[pixel] = closestColor;
  }
  for (auto& pixel : pixels) {
    if (replacementMap.find(pixel) == replacementMap.end()) {
      pixel = replacementMap[pixel];
    }
  }
}
