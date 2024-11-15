#ifndef CUTFREQ_HPP
#define CUTFREQ_HPP

#include <iostream>
#include "imagesoa.hpp"
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <limits>
#include <functional>

constexpr int sixteen = 16;
constexpr int eight = 8;
constexpr int mask = 0xFF;

template <typename T>
void clearToBlack(ImageSOA<T> & pixels) {
  std::fill(pixels.r.begin(), pixels.r.end(), static_cast<T>(0));
  std::fill(pixels.g.begin(), pixels.g.end(), static_cast<T>(0));
  std::fill(pixels.b.begin(), pixels.b.end(), static_cast<T>(0));
}

inline bool comparePixels(std::pair<int, int> const & pixel1, std::pair<int, int> const & pixel2) {
  if (pixel1.second != pixel2.second) {
    return pixel1.second < pixel2.second;  // Lower frequency comes first
  }

  // For equal frequency, prioritize by b, then g, then r
  int const pixel1B = (pixel1.first & mask);
  if (int const pixel2B = (pixel2.first & mask); pixel1B != pixel2B) { return pixel1B > pixel2B; }

  int const pixel1G = (pixel1.first >> eight) & mask;
  if (int const pixel2G = (pixel2.first >> eight) & mask; pixel1G != pixel2G) { return pixel1G > pixel2G; }

  int const pixel1R = (pixel1.first >> sixteen) & mask;
  int const pixel2R = (pixel2.first >> sixteen) & mask;

  return pixel1R > pixel2R;
}

inline std::vector<std::pair<int, int>> sortFrequency(const std::map<int, int>& frequency) {
  std::vector<std::pair<int, int>> frequencyVector(frequency.begin(), frequency.end());
  std::ranges::sort(frequencyVector, comparePixels);
  return frequencyVector;
}

// KDTree class to handle nearest neighbor searches
class KDTree {
private:
    std::vector<int> tree; // Tree storing pixel colors as packed integers

    // Helper functions to extract RGB components from a packed pixel integer
    [[nodiscard]] static int extractRed(int const pixel) {
        return (pixel >> sixteen) & mask;
    }

    [[nodiscard]] static int extractGreen(int const pixel) {
        return (pixel >> eight) & mask;
    }

    [[nodiscard]] static int extractBlue(int const pixel) {
        return pixel & mask;
    }

    // Comparator function for sorting pixels based on the current axis
  [[nodiscard]] static bool comparator(std::vector<int> const &pixels, int const axis) {
      switch (axis) {
        case 0: return extractRed(pixels[0]) < extractRed(pixels[1]);
        case 1: return extractGreen(pixels[0]) < extractGreen(pixels[1]);
        case 2: return extractBlue(pixels[0]) < extractBlue(pixels[1]);
        default:
          break;
      }
      return false;
    }
    //NOLINTBEGIN(misc-no-recursion)
    // Recursive function to build the KDTree
    void buildTree(size_t const currentStart, size_t const currentEnd, int const axis) {
        if (currentStart >= currentEnd) { return;
}

        using DifferenceType = std::vector<int>::difference_type;

        size_t const median = (currentStart + currentEnd) / 2;

        std::nth_element(
            tree.begin() + static_cast<DifferenceType>(currentStart),
            tree.begin() + static_cast<DifferenceType>(median),
            tree.begin() + static_cast<DifferenceType>(currentEnd),
            [&](int const pixel1, int const pixel2) { return comparator({pixel1, pixel2}, axis); });

        buildTree(currentStart, median, (axis + 1) % 3);
        buildTree(median + 1, currentEnd, (axis + 1) % 3);
    }
  //NOLINTEND(misc-no-recursion)

public:

    // Constructor to initialize and build the KDTree
    explicit KDTree(const std::vector<int>& pixels) : tree(pixels) {

        buildTree(0, tree.size(), 0);
    }

    // Function to find the nearest neighbor to a given pixel
    [[nodiscard]] int nearestNeighbor(int pixel) const {
        int nearest = tree[0];
        double minDist = std::numeric_limits<double>::max();

        for (const auto& candidate : tree) {
          double const dist = std::sqrt(
                std::pow(extractRed(pixel) - extractRed(candidate), 2) +
                std::pow(extractGreen(pixel) - extractGreen(candidate), 2) +
                std::pow(extractBlue(pixel) - extractBlue(candidate), 2));

            if (dist < minDist) {
                minDist = dist;
                nearest = candidate;
            }
        }

        return nearest;
    }
};

// Function to remove the n least frequent colors (LFC) and replace them with the nearest neighbor from the KDTree
template <typename T>
void removeLFCsoa(ImageSOA<T>& pixels, int n) {
    std::map<int, int> frequency;
    for (size_t i = 0; i < pixels.size(); ++i) {
        int const pixel = (pixels.r[i] << sixteen) | (pixels.g[i] << eight) | pixels.b[i]; // Pack RGB
        ++frequency[pixel];
    }
    // Handle case where n >= number of unique colors
    if (n >= static_cast<int>(frequency.size())) {
        clearToBlack(pixels);
        return;
    }
    std::vector<std::pair<int, int>> frequencyVector = sortFrequency(frequency);
    std::vector<int> removed_pixels;
    std::vector<int> remainingColors;
    for (size_t i = 0; i < frequencyVector.size(); ++i) {
        if (i < static_cast<size_t>(n)) {
            removed_pixels.push_back(frequencyVector[i].first);
        } else {
            remainingColors.push_back(frequencyVector[i].first);
        }
    }
    KDTree const kdTree(remainingColors);
    std::map<int, int> replacementMap;
    for (const auto& pixel : removed_pixels) {
        replacementMap[pixel] = kdTree.nearestNeighbor(pixel);
    }
    // Step 6: Replace colors in the original image
    for (size_t i = 0; i < pixels.size(); ++i) {
        int const pixel = (pixels.r[i] << sixteen) | (pixels.g[i] << eight) | pixels.b[i];
        auto replacement = replacementMap.find(pixel);
        if (replacement != replacementMap.end()) {
            int const newPixel = replacement->second;
            pixels.r[i] = static_cast<T>((newPixel >> sixteen) & mask);  // Extract red
            pixels.g[i] = static_cast<T>((newPixel >> eight) & mask);   // Extract green
            pixels.b[i] = static_cast<T>(newPixel & mask);          // Extract blue
        }
    }
}

#endif //CUTFREQ_HPP