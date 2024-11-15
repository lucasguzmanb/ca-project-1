#ifndef RESIZE_HPP
#define RESIZE_HPP

#include <vector>
#include "imagesoa.hpp"

struct dimensions {
  int coordinate;
  int dimension;
  int newDimension;
};

struct coordinates {
    int j;
    int i;
    int x_l;
    int y_l;
    int x_h;
    int y_h;
};


template <typename T>
ImageSOA<T> resize(ImageSOA<T> pixels, Metadata const & metadata, std::vector<int> const & size);

template <typename T>
T interpolate(T color1, T color2, double frac);

template <typename T>
T interpolation(std::vector<T> &colorPixels, coordinates coordinate,
                   std::vector<int> const & widths, std::vector<double> const & diff);

#endif // RESIZE_HPP