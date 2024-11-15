#ifndef RESIZE_HPP
#define RESIZE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "imagesoa.hpp"



template <typename T>
ImageSOA<T> resize(ImageSOA<T> pixels, Metadata metadata, std::vector<int> const & size);

#endif // RESIZE_HPP
