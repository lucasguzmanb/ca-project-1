#include "cutfreq.hpp"

struct RGB {
  std::vector<int> r;
  std::vector<int> g;
  std::vector<int> b;
};
std::tuple<int , int, int> getColor(const RGB& image, int indx) {
  return {image.r[indx], image.g[indx], image.b[indx]};
}
double euclideanDistance(int r1, int g1, int b1, int r2, int g2, int b2) {
  return std::sqrt(std::pow(r1-r2, 2) + std::pow(g1-g2, 2) + std::pow(b1-b2, 2));
}
void removeLFCsoa(RGB& image, int n) {
  for (size_t i = 0; i < image.r.size(); ++i) {
    frequency[getColor(image, i)]++;
  }
  std::vector<std::pair<std::tuple<int, int, int>, int>> colorFreq(frequency.begin(), frequency.end());
  std::sort(colorFreq.begin(), colorFreq.end(), [](const auto& a, const auto& b) {
    if (a.second < b.second) return a.second < b.second; //Sort by frequency
    return std::tie(std::get<2>(b.first), std::get<1>(b.first), std::get<0>(b.first)) < std::tie(std::get<2>(a.first), std::get<1>(a.first), std::get<0>(a.first));
  });
  std::vector<std::tuple<int, int, int>> newColor(colorFreq.begin(), removed_pixels;
  for (int i = 0; i < n && i < colorFreq.size(); ++i) {
    removed_pixels.push_back(colorFreq[i].first);
  }
  std::unordered_map<std::tuple<int, int, int>, std::<std::tuple<int, int, int>> > replacementMap;
  for (const auto& pixel : removed_pixels) {
    double minDistance = std::numeric_limits<double>::max();
    std::tuple<int, int, int> closestColor = pixel;
    for (const auto& [remainingColor, freq] : colorFreq) {
      if (std::find(removed_pixels.begin(), removed_pixels.end(), remainingColor) == removed_pixels.end()) {
        auto [r1, g1, b1] = pixel;
        auto [r2, g2, b2] = remainingColor;
        double dist = euclideanDistance(r1, g1, b1, r2, g2, b2);
        if (dist < minDistance) {
          minDistance = dist;
          closestColor = remainingColor;
        }
      }
    }
    replacementMap[pixel] = closestColor;
  }
  for (size_t i = 0; i < image.r.size(); ++i) {
    auto color = getColor(image, i);
    if (replacementMap.find(color) != replacementMap.end()) {
      auto [r, g, b] = replacementMap[color];
      image.r[i] = r;
      image.g[i] = g;
      image.b[i] = b;
    }
  }
}
