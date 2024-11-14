#ifndef MAXLEVEL_AOS_HPP
#define MAXLEVEL_AOS_HPP

#include "imageaos.hpp"

#include <vector>

template <typename T, typename U>
std::vector<Pixel<U>> maxlevel(std::vector<Pixel<T>> inputImage, int oldLevel, int newLevel);

#endif  // MAXLEVEL_AOS_HPP