#ifndef MAXLEVEL_SOA_HPP
#define MAXLEVEL_SOA_HPP
#include "imagesoa.hpp"

template <typename T>
ImageSOA<T> maxlevel(ImageSOA<T> inputImage, int oldLevel, int newLevel);

#endif //MAXLEVEL_SOA_HPP