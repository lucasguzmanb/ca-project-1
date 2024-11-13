
#ifndef CUTFREQ_HPP
#define CUTFREQ_HPP

#include "cutfreq.hpp"
#include <vector>
#include <tuple>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <functional>

template <typename T>
class KDTree {
private:
    std::vector<int> tree;  // Tree storing the pixel colors

    // Helper functions to extract RGB components from a packed pixel integer
    int extractRed(int pixel) const {
        return (pixel >> 16) & 0xFF;  // Extract the red component
    }

    int extractGreen(int pixel) const {
        return (pixel >> 8) & 0xFF;   // Extract the green component
    }

    int extractBlue(int pixel) const {
        return pixel & 0xFF;          // Extract the blue component
    }

    // Function to calculate the Euclidean distance between two pixels (RGB)
    double euclideanDistance(int pixel1, int pixel2) const {
        int r1 = extractRed(pixel1);
        int g1 = extractGreen(pixel1);
        int b1 = extractBlue(pixel1);

        int r2 = extractRed(pixel2);
        int g2 = extractGreen(pixel2);
        int b2 = extractBlue(pixel2);

        return std::sqrt((r1 - r2) * (r1 - r2) +
                         (g1 - g2) * (g1 - g2) +
                         (b1 - b2) * (b1 - b2));
    }

    // Function to recursively build the KD-Tree
    void buildTree(size_t currentStart, size_t currentEnd, int axis) {
        if (currentStart + 1 >= currentEnd) return;

        size_t median = currentStart + (currentEnd - currentStart) / 2;

        // Sort by the current axis (red=0, green=1, blue=2)
      using DifferenceType = typename std::vector<int>::difference_type;

      std::nth_element(
          tree.begin() + static_cast<DifferenceType>(currentStart),
          tree.begin() + static_cast<DifferenceType>(median),
          tree.begin() + static_cast<DifferenceType>(currentEnd),
            [&](int a, int b) {
                if (axis == 0) return extractRed(a) < extractRed(b);
                if (axis == 1) return extractGreen(a) < extractGreen(b);
                return extractBlue(a) < extractBlue(b);
            });

        // Recursively build the left and right subtrees
        buildTree(currentStart, median, (axis + 1) % 3);
        buildTree(median + 1, currentEnd, (axis + 1) % 3);
    }

    // Recursive function for nearest neighbor search
    void searchNearest(int pixel, int& nearest, double& minDist, size_t start, size_t end, int axis) const {
        if (start >= end) return;

        size_t median = start + (end - start) / 2;
        int currentPixel = tree[median];
        double dist = euclideanDistance(pixel, currentPixel);

        if (dist < minDist) {
            minDist = dist;
            nearest = currentPixel;
        }

        // Determine which subtree to search
        int pixelValue = (axis == 0) ? extractRed(pixel) :
                         (axis == 1) ? extractGreen(pixel) :
                                       extractBlue(pixel);
        int medianValue = (axis == 0) ? extractRed(currentPixel) :
                          (axis == 1) ? extractGreen(currentPixel) :
                                        extractBlue(currentPixel);

        if (pixelValue < medianValue) {
            searchNearest(pixel, nearest, minDist, start, median, (axis + 1) % 3);
            if (std::abs(pixelValue - medianValue) < minDist) {
                searchNearest(pixel, nearest, minDist, median + 1, end, (axis + 1) % 3);
            }
        } else {
            searchNearest(pixel, nearest, minDist, median + 1, end, (axis + 1) % 3);
            if (std::abs(pixelValue - medianValue) < minDist) {
                searchNearest(pixel, nearest, minDist, start, median, (axis + 1) % 3);
            }
        }
    }

public:
    // Constructor: Initializes the tree from a vector of color pixels
    KDTree(const std::vector<int>& pixels) {
        if (pixels.empty()) {
            throw std::invalid_argument("Cannot build KDTree with empty input.");
        }
        tree = pixels;
        buildTree(0, tree.size(), 0);  // Start building the tree along the red axis
    }

    // Function to find the nearest neighbor to the given pixel color
    int nearestNeighbor(int pixel) const {
        int nearest = tree[0];
        double minDist = std::numeric_limits<double>::max();
        searchNearest(pixel, nearest, minDist, 0, tree.size(), 0);
        return nearest;
    }
};




// Function to remove least frequent colors (LFC) and replace them with the nearest neighbor from the KDTree
template <typename T>
void removeLFCsoa(ImageSOA<T>& pixels, int n) {
    // Create frequency map
  auto start = std::chrono::high_resolution_clock::now();
    std::map<int, int> frequency;
    for (size_t i = 0; i < pixels.size(); ++i) {
        int pixel = (pixels.r[i] << 16) | (pixels.g[i] << 8) | pixels.b[i]; // Combine RGB
        ++frequency[pixel];
    }

    // Handle the case where n >= unique colors
    if (n >= static_cast<int>(frequency.size())) {
        std::fill(pixels.r.begin(), pixels.r.end(), static_cast<T>(0));
        std::fill(pixels.g.begin(), pixels.g.end(), static_cast<T>(0));
        std::fill(pixels.b.begin(), pixels.b.end(), static_cast<T>(0));
        std::cout << "All pixels changed to black as n >= unique colors.\n";
        return;
    }

    // Sort and remove least frequent colors
    std::vector<std::pair<int, int>> frequencyVector(frequency.begin(), frequency.end());
    std::nth_element(
        frequencyVector.begin(), frequencyVector.begin() + n, frequencyVector.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; }
    );

    std::vector<int> removed_pixels, remainingColors;
    for (size_t i = 0; i < frequencyVector.size(); ++i) {
        if (i < static_cast<size_t>(n)) {
            removed_pixels.push_back(frequencyVector[i].first);
        } else {
            remainingColors.push_back(frequencyVector[i].first);
        }
    }

    // Construct KDTree and create replacement map
    KDTree<int> kdTree(remainingColors);
    std::map<int, int> replacementMap;
    for (const auto& pixel : removed_pixels) {
        replacementMap[pixel] = kdTree.nearestNeighbor(pixel);
    }

    // Replace pixels
    for (size_t i = 0; i < pixels.size(); ++i) {
        int pixel = (pixels.r[i] << 16) | (pixels.g[i] << 8) | pixels.b[i];
        auto it = replacementMap.find(pixel);
        if (it != replacementMap.end()) {
            int newPixel = it->second;
            pixels.r[i] = static_cast<T>((newPixel >> 16) & 0xFF);
            pixels.g[i] = static_cast<T>((newPixel >> 8) & 0xFF);
            pixels.b[i] = static_cast<T>(newPixel & 0xFF);
        }
    }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Execution time: " << duration.count() << "\n";
}






#endif //CUTFREQ_HPP
