#include "cutfreq.hpp"
#include <cmath>
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>
#include <tuple>

template <typename T>
struct RGB {
    T r, g, b;

    bool operator==(const RGB<T>& other) const {
        return std::tie(r, g, b) == std::tie(other.r, other.g, other.b);
    }
};

template <typename T>
struct RGB_map {
    std::size_t operator()(const RGB<T>& color) const {
        return std::hash<T>{}(color.r) ^ std::hash<T>{}(color.g) ^ std::hash<T>{}(color.b);
    }
};

template <typename T>
double euclideanDistance(const RGB<T>& first, const RGB<T>& second) {
    return std::sqrt(std::pow(first.r - second.r, 2) + std::pow(first.g - second.g, 2) + std::pow(first.b - second.b, 2));
}

template <typename T>
void removeLFCaos(std::vector<RGB<T>>& pixels, int n) {
    std::unordered_map<RGB<T>, int, RGB_map<T>> frequency;

    // Calculate frequency of each color
    for (const auto& pixel : pixels) {
        frequency[pixel]++;
    }

    // Create a vector of color frequencies
    std::vector<std::pair<RGB<T>, int>> colorFreq(frequency.begin(), frequency.end());
    std::sort(colorFreq.begin(), colorFreq.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second < b.second; // Sort by frequency
        return std::tie(b.first.b, b.first.g, b.first.r) < std::tie(a.first.b, a.first.g, a.first.r); // Tie-breaking by b, g, r
    });

    // Collect least frequent colors
    std::vector<RGB<T>> removed_pixels;
    for (int i = 0; i < n && i < colorFreq.size(); i++) {
        removed_pixels.push_back(colorFreq[i].first);
    }

    // Map to replace least frequent colors with closest colors
    std::unordered_map<RGB<T>, RGB<T>, RGB_map<T>> replacementMap;
    for (const auto& pixel : removed_pixels) {
        double minDistance = std::numeric_limits<double>::max();
        RGB<T> closestColor = pixel;

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

    // Replace pixels in the original image
    for (auto& pixel : pixels) {
        auto it = replacementMap.find(pixel);
        if (it != replacementMap.end()) {
            pixel = it->second; // Update pixel color
        }
    }
}


