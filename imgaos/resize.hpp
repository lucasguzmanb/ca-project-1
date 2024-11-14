#ifndef RESIZE_AOS_HPP
#define RESIZE_AOS_HPP

#include "imageaos.hpp"
#include "common/binaryio.hpp"

#include <vector>

// Function for pixel interpolation
template <typename T>
Pixel<T> interpolation(Pixel<T> color1, Pixel<T> color2, double frac);

template <typename T>
std::vector<Pixel<T>> resize(std::vector<Pixel<T>> pixels, Metadata & metadata,
                             std::vector<int> const & size);
#endif // RESIZE_AOS_HPP