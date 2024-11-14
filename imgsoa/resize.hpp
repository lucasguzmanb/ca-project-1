#ifndef RESIZE_SOA_HPP
#define RESIZE_SOA_HPP

#include <vector>
#include "imagesoa.hpp"

template <typename T>
ImageSOA<T> resize(ImageSOA<T> pixels, Metadata & metadata, std::vector<int> const & size);

template <typename T>
T interpolation(T color1, T color2, double frac);


#endif // RESIZE_SOA_HPP