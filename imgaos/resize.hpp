#ifndef RESIZE_HPP
#define RESIZE_HPP

#include "imageaos.hpp"

#include <vector>

// Function for pixel interpolation

template <typename T>
std::vector<Pixel<T>> resize(std::vector<Pixel<T> > pixels, Metadata const & metadata,
                             std::vector<int> const & size);

template <typename T>
Pixel<T> interpolation(Pixel<T> color1, Pixel<T> color2, double frac);

#endif // RESIZE_HPP