#ifndef RESIZE_HPP
#define RESIZE_HPP

#include "imageaos.hpp"

#include <iostream>
#include <fstream>
#include <vector>

// Function for pixel interpolation

template <typename T>
std::vector<Pixel<T>> resize(std::vector<Pixel<T> > pixels, Metadata const & metadata,
                             std::vector<int> const & size);

#endif // RESIZE_HPP