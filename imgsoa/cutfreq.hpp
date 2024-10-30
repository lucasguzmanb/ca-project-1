
#ifndef CUTFREQ_HPP
#define CUTFREQ_HPP

#include "cutfreq.hpp"
#include <vector>
#include <tuple>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <limits>

template <typename T>
struct RGB {
    std::vector<T> r;
    std::vector<T> g;
    std::vector<T> b;
};

template <typename T>
std::tuple<T, T, T> getColor(const RGB<T>& image, size_t indx) {
    return {image.r[indx], image.g[indx], image.b[indx]};
}

template <typename T>
double euclideanDistance(T r1, T g1, T b1, T r2, T g2, T b2) {
    return std::sqrt(std::pow(r1 - r2, 2) + std::pow(g1 - g2, 2) + std::pow(b1 - b2, 2));
}

template <typename T>
void removeLFCsoa(RGB<T>& image, int n) {
    std::unordered_map<std::tuple<T, T, T>, int> frequency;

    // Calculate frequency of each color
    for (size_t i = 0; i < image.r.size(); ++i) {
        frequency[getColor(image, i)]++;
    }

    // Create a vector of color frequencies
    std::vector<std::pair<std::tuple<T, T, T>, int>> colorFreq(frequency.begin(), frequency.end());
    std::sort(colorFreq.begin(), colorFreq.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second < b.second; // Sort by frequency
        return std::tie(std::get<2>(b.first), std::get<1>(b.first), std::get<0>(b.first)) <
               std::tie(std::get<2>(a.first), std::get<1>(a.first), std::get<0>(a.first)); // Tie-breaking by b, g, r
    });

    // Collect least frequent colors
    std::vector<std::tuple<T, T, T>> removed_pixels;
    for (int i = 0; i < n && i < colorFreq.size(); ++i) {
        removed_pixels.push_back(colorFreq[i].first);
    }

    // Map to replace least frequent colors with closest colors
    std::unordered_map<std::tuple<T, T, T>, std::tuple<T, T, T>> replacementMap;
    for (const auto& pixel : removed_pixels) {
        double minDistance = std::numeric_limits<double>::max();
        std::tuple<T, T, T> closestColor = pixel;

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

    // Replace pixels in the original image
    for (size_t i = 0; i < image.r.size(); ++i) {
        auto color = getColor(image, i);
        auto it = replacementMap.find(color);
        if (it != replacementMap.end()) {
            auto [r, g, b] = it->second;
            image.r[i] = r;
            image.g[i] = g;
            image.b[i] = b;
        }
    }
}

#endif //CUTFREQ_HPP
