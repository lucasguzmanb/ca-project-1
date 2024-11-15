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
#include <stack>
#include <vector>
template<typename T>
double euclideanDistance(Pixel<T> const & pixel1, Pixel<T> const & pixel2) {
  return std::sqrt(std::pow(pixel1.r - pixel2.r, 2) + std::pow(pixel1.g - pixel2.g, 2) +
                   std::pow(pixel1.b - pixel2.b, 2));
}

template <typename T>
void updateBestMatch(Pixel<T> const & currentPixel, Pixel<T> const & target, Pixel<T> & bestMatch,
                     double & bestDistance) {
  double const dist = euclideanDistance(currentPixel, target);
  if (dist < bestDistance) {
    bestDistance = dist;
    bestMatch    = currentPixel;
  }
}

template <typename T>
double computeAxisDistance(Pixel<T> const &currentPixel, Pixel<T> const &target, int const axis) {
  switch (axis) {
    case 0: return std::abs(target.r - currentPixel.r);
    case 1: return std::abs(target.g - currentPixel.g);
    case 2:
      return std::abs(target.b - currentPixel.b);
    default:
      break;
  }
  return NAN;
}

template <typename T>
std::tuple<size_t, size_t, size_t, size_t> determineTraversalOrder(
    std::vector<size_t> const &bounds, Pixel<T> const &currentPixel,
    Pixel<T> const &target, int const axis) {

  size_t const start = bounds[0];
  size_t const end = bounds[1];
  size_t const median = bounds[2];

  bool const leftFirst = (axis == 0 && target.r < currentPixel.r) ||
                         (axis == 1 && target.g < currentPixel.g) ||
                         (axis == 2 && target.b < currentPixel.b);

  size_t nearStart = 0;
  size_t nearEnd = 0;
  size_t farStart = 0;
  size_t farEnd = 0;

  if (leftFirst) {
    nearStart = start;
    nearEnd = median;
    farStart = median + 1;
    farEnd = end;
  } else {
    nearStart = median + 1;
    nearEnd = end;
    farStart = start;
    farEnd = median;
  }

  return {nearStart, nearEnd, farStart, farEnd};
}


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
      std::vector<size_t> const bounds = {0, tree.size(), 0};
      nearestNeighborSearch(bounds, target, bestMatch, bestDistance);
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
  void nearestNeighborSearch(std::vector<size_t> const &bounds, Pixel<T> const &target,
                           Pixel<T> &bestMatch, double &bestDistance) const {
      struct StackEntry {
        std::vector<size_t> bounds;
        int depth{};
      };

      std::stack<StackEntry> stack;
      stack.push({bounds, 0});

      while (!stack.empty()) {
        StackEntry current = stack.top();
        stack.pop();

        size_t const start = current.bounds[0];
        size_t const end = current.bounds[1];
        size_t median = current.bounds[2];

        if (start >= end) { continue;
}

        Pixel<T> const &currentPixel = tree[median];
        updateBestMatch(currentPixel, target, bestMatch, bestDistance);

        int const axis = current.depth % 3;
        double const axisDist = computeAxisDistance(currentPixel, target, axis);

        auto [nearStart, nearEnd, farStart, farEnd] = determineTraversalOrder(current.bounds, currentPixel, target, axis);

        if (axisDist < bestDistance) {
          stack.push({{farStart, farEnd, median}, current.depth + 1});
}
        stack.push({{nearStart, nearEnd, median}, current.depth + 1});
      }
    }

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