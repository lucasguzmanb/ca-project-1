#ifndef RESIZE_HPP
#define RESIZE_HPP

#include <iostream>
#include <fstream>
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

#endif // RESIZE_HPP
