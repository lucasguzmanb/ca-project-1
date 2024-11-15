#ifndef CUTFREQ_AOS_HPP
#define CUTFREQ_AOS_HPP

#include "imageaos.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <tuple>
#include <vector>

// Assume Pixel<T> and Pixel_map<T> are already defined, as in the original code.

template <typename T>
class KDTree {
  public:
    explicit KDTree(std::vector<Pixel<T>> const & points) : tree(points) {
      buildTree(0, tree.size(), 0);
    }

    // Find the nearest neighbor to a given pixel in the KD-Tree
    [[nodiscard]] Pixel<T> nearestNeighbor(Pixel<T> const & target) const {
      Pixel<T> bestMatch{};
      double bestDistance = std::numeric_limits<double>::max();
      nearestNeighborSearch({0, tree.size(), 0}, target, bestMatch, bestDistance);
      return bestMatch;
    }

  private:
    std::vector<Pixel<T>> tree;

    // Build KD-Tree in place with an optimized iterative structure
    void buildTree(size_t start, size_t end, int depth) {
      std::vector<std::tuple<size_t, size_t, int>> stack;
      stack.emplace_back(start, end, depth);

      while (!stack.empty()) {
        auto [currentStart, currentEnd, currentDepth] = stack.back();
        stack.pop_back();

        if (currentStart >= currentEnd) { continue; }

        int const axis  = currentDepth % 3;
        auto comparator = [axis](Pixel<T> const & pixel1, Pixel<T> const & pixel2) {
          if (axis == 0) { return pixel1.r < pixel2.r; }
          if (axis == 1) { return pixel1.g < pixel2.g; }
          return pixel1.b < pixel2.b;
        };
        size_t const median = (currentStart + currentEnd) / 2;
        std::nth_element(
            tree.begin() +
                static_cast<typename std::vector<Pixel<T>>::difference_type>(currentStart),
            tree.begin() + static_cast<typename std::vector<Pixel<T>>::difference_type>(median),
            tree.begin() + static_cast<typename std::vector<Pixel<T>>::difference_type>(currentEnd),
            comparator);

        // Push left and right subtrees onto the stack
        stack.emplace_back(currentStart, median, currentDepth + 1);
        stack.emplace_back(median + 1, currentEnd, currentDepth + 1);
      }
    }
    // Optimized nearest neighbor search
  // NOLINTBEGIN(misc-no-recursion)
  void nearestNeighborSearch(std::vector<size_t> const &bounds, Pixel<T> const &target, Pixel<T> &bestMatch, double &bestDistance) const {
      size_t const start = bounds[0];
      size_t const end = bounds[1];
      int const depth = static_cast<int>(bounds[2]);
      if (start >= end) {
        return;
      }
      size_t const median = (start + end) / 2;
      Pixel<T> const &current = tree[median];
      // Compute distance and update the best match if necessary
      if (double const dist = euclideanDistance(current, target); dist < bestDistance) {
        bestDistance = dist;
        bestMatch = current;
      }
      // Determine the splitting axis and calculate the axis distance
      int const axis = depth % 3;
      double axisDist = NAN;
      if (axis == 0) {
        axisDist = std::abs(target.r - current.r);
      } else if (axis == 1) {
        axisDist = std::abs(target.g - current.g);
      } else {
        axisDist = std::abs(target.b - current.b);
      }
      // Determine traversal order
      bool const leftFirst = (axis == 0 && target.r < current.r) ||
                             (axis == 1 && target.g < current.g) ||
                             (axis == 2 && target.b < current.b);
      std::vector<size_t> const nearBounds = {leftFirst ? start : median + 1,
                                        leftFirst ? median : end,
                                        static_cast<size_t>(depth + 1)};
      std::vector<size_t> const farBounds = {leftFirst ? median + 1 : start,
                                       leftFirst ? end : median,
                                       static_cast<size_t>(depth + 1)};
      // Recursively search the closer subtree
      nearestNeighborSearch(nearBounds, target, bestMatch, bestDistance);
      // Only search the farther subtree if there's a possibility of a closer match
      if (axisDist < bestDistance) {
        nearestNeighborSearch(farBounds, target, bestMatch, bestDistance);
      }
    }
    // Calculate Euclidean distance between two pixels
    [[nodiscard]] double euclideanDistance(Pixel<T> const & pixel1, Pixel<T> const & pixel2) const {
      return std::sqrt(std::pow(pixel1.r - pixel2.r, 2) + std::pow(pixel1.g - pixel2.g, 2) +
                       std::pow(pixel1.b - pixel2.b, 2));
    }
  // NOLINTEND(misc-no-recursion)
};

template <typename T>
void removeLFCaos(std::vector<Pixel<T>> & pixels, int n) {
  std::map<Pixel<T>, int, Pixel_map<T>> frequency;
  for (auto const & pixel : pixels) { ++frequency[pixel]; }
  // Check if n is greater than the number of unique colors
  if (n >= static_cast<int>(frequency.size())) {
    std::fill(pixels.begin(), pixels.end(), Pixel<T>{0, 0, 0});
    std::cout << "All pixels changed to black as n >= unique colors.\n";
    return;
  }
  // Sort colors by frequency, breaking ties by higher b, g, and r values
  std::vector<std::pair<Pixel<T>, int>> frequencyVector(frequency.begin(), frequency.end());
  std::sort(frequencyVector.begin(), frequencyVector.end(),
            [](auto const & pair1, auto const & pair2) {
              return std::tie(pair1.second, pair1.first.b, pair1.first.g, pair1.first.r) <
                     std::tie(pair2.second, pair2.first.b, pair2.first.g, pair2.first.r);
            });
  std::vector<Pixel<T>> removed_pixels;
  std::vector<Pixel<T>> remainingColors;
  for (size_t i = 0; i < frequencyVector.size(); ++i) {
    if (i < static_cast<size_t>(n)) {
      removed_pixels.push_back(frequencyVector[i].first);
    } else {
      remainingColors.push_back(frequencyVector[i].first);
    }
  }
  // Step 3: Build k-d tree for remaining colors
  KDTree<T> const kdTree(remainingColors);
  // Step 4: Map removed pixels to their closest color using the KD-Tree
  std::map<Pixel<T>, Pixel<T>, Pixel_map<T>> replacementMap;
  for (auto const & pixel : removed_pixels) {
    replacementMap[pixel] = kdTree.nearestNeighbor(pixel);
  }
  // Step 5: Replace pixels in the original image
  for (auto & pixel : pixels) {
    auto replacement = replacementMap.find(pixel);
    if (replacement != replacementMap.end()) { pixel = replacement->second; }
  }
}

#endif  // CUTFREQ_AOS_HPP