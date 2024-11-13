#ifndef CUTFREQ_AOS_HPP
#define CUTFREQ_AOS_HPP

#include "imageaos.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <queue>
#include <map>
#include <unordered_map>
#include <vector>
#include <limits>
#include <chrono>



#include <cmath>
#include <limits>
#include <map>
#include <vector>
#include <algorithm>
#include <tuple>
#include <chrono>
#include <iostream>

// Assume Pixel<T> and Pixel_map<T> are already defined, as in the original code.

template <typename T>
class KDTree {
public:
    KDTree(const std::vector<Pixel<T>>& points) : tree(points) {
        buildTree(0, tree.size(), 0);
    }

    // Find the nearest neighbor to a given pixel in the KD-Tree
    Pixel<T> nearestNeighbor(const Pixel<T>& target) const {
        Pixel<T> bestMatch;
        double bestDistance = std::numeric_limits<double>::max();
        nearestNeighborSearch(0, tree.size(), 0, target, bestMatch, bestDistance);
        return bestMatch;
    }

private:
    std::vector<Pixel<T>> tree;

    // Build KD-Tree in place with an optimized iterative structure
    void buildTree(size_t start, size_t end, int depth) {
        std::vector<std::tuple<size_t, size_t, int>> stack;
        stack.push_back(std::make_tuple(start, end, depth));

        while (!stack.empty()) {
            auto [currentStart, currentEnd, currentDepth] = stack.back();
            stack.pop_back();

            if (currentStart >= currentEnd) continue;

            int axis = currentDepth % 3;
            auto comparator = [axis](const Pixel<T>& a, const Pixel<T>& b) {
                return (axis == 0) ? a.r < b.r : (axis == 1) ? a.g < b.g : a.b < b.b;
            };

            size_t median = (currentStart + currentEnd) / 2;
            std::nth_element(tree.begin() + static_cast<typename std::vector<Pixel<T>>::difference_type>(currentStart),
                             tree.begin() + static_cast<typename std::vector<Pixel<T>>::difference_type>(median),
                             tree.begin() + static_cast<typename std::vector<Pixel<T>>::difference_type>(currentEnd), comparator);

            // Push left and right subtrees onto the stack
            stack.push_back(std::make_tuple(currentStart, median, currentDepth + 1));
            stack.push_back(std::make_tuple(median + 1, currentEnd, currentDepth + 1));
        }
    }

    // Optimized nearest neighbor search
    void nearestNeighborSearch(size_t start, size_t end, int depth, const Pixel<T>& target,
                               Pixel<T>& bestMatch, double& bestDistance) const {
        if (start >= end) return;

        int axis = depth % 3;
        size_t median = (start + end) / 2;
        const Pixel<T>& current = tree[median];

        double dist = euclideanDistance(current, target);
        if (dist < bestDistance) {
            bestDistance = dist;
            bestMatch = current;
        }

        // Decide which subtree to search first based on the current splitting axis
        bool leftFirst = (axis == 0 && target.r < current.r) ||
                         (axis == 1 && target.g < current.g) ||
                         (axis == 2 && target.b < current.b);

        size_t nearStart = leftFirst ? start : median + 1;
        size_t nearEnd = leftFirst ? median : end;
        size_t farStart = leftFirst ? median + 1 : start;
        size_t farEnd = leftFirst ? end : median;

        // Search the closer subtree first
        nearestNeighborSearch(nearStart, nearEnd, depth + 1, target, bestMatch, bestDistance);

        // If there's a chance the other side could contain a closer point, search it
        double axisDist = std::abs((axis == 0 ? target.r - current.r
                                 : axis == 1 ? target.g - current.g
                                             : target.b - current.b));

        if (axisDist < bestDistance) {
            nearestNeighborSearch(farStart, farEnd, depth + 1, target, bestMatch, bestDistance);
        }
    }

    // Calculate Euclidean distance between two pixels
    double euclideanDistance(const Pixel<T>& a, const Pixel<T>& b) const {
        return std::sqrt(std::pow(a.r - b.r, 2) + std::pow(a.g - b.g, 2) + std::pow(a.b - b.b, 2));
    }
};

template <typename T>
void removeLFCaos(std::vector<Pixel<T>>& pixels, int n) {
  auto start = std::chrono::high_resolution_clock::now();

  // Step 1: Calculate frequency of each color
  std::map<Pixel<T>, int, Pixel_map<T>> frequency;
  for (const auto& pixel : pixels) {
    ++frequency[pixel];
  }

  // Check if n is greater than the number of unique colors
  if (n >= static_cast<int>(frequency.size())) {
    std::fill(pixels.begin(), pixels.end(), Pixel<T>{0, 0, 0});
    std::cout << "All pixels changed to black as n >= unique colors.\n";
    return;
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Frequency map: " << duration.count() << "\n";

  // Step 2: Sort colors by frequency, breaking ties by higher b, g, and r values
  std::vector<std::pair<Pixel<T>, int>> frequencyVector(frequency.begin(), frequency.end());
  std::sort(frequencyVector.begin(), frequencyVector.end(),
            [](const auto& pair1, const auto& pair2) {
                return std::tie(pair1.second, pair1.first.b, pair1.first.g, pair1.first.r) <
                       std::tie(pair2.second, pair2.first.b, pair2.first.g, pair2.first.r);
            });

  std::vector<Pixel<T>> removed_pixels, remainingColors;
  for (size_t i = 0; i < frequencyVector.size(); ++i) {
    if (i < static_cast<size_t>(n)) {
      removed_pixels.push_back(frequencyVector[i].first);
    } else {
      remainingColors.push_back(frequencyVector[i].first);
    }
  }

  // Step 3: Build k-d tree for remaining colors
  KDTree<T> kdTree(remainingColors);

  // Step 4: Map removed pixels to their closest color using the KD-Tree
  std::map<Pixel<T>, Pixel<T>, Pixel_map<T>> replacementMap;
  for (const auto& pixel : removed_pixels) {
    replacementMap[pixel] = kdTree.nearestNeighbor(pixel);
  }

  // Step 5: Replace pixels in the original image
  for (auto& pixel : pixels) {
    auto it = replacementMap.find(pixel);
    if (it != replacementMap.end()) {
      pixel = it->second;
    }
  }

  end = std::chrono::high_resolution_clock::now();
  duration = end - start;
  std::cout << "Execution finished: " << duration.count() << "\n";
}



#endif  // CUTFREQ_AOS_HPP