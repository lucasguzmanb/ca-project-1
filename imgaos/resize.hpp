#ifndef RESIZE_HPP
#define RESIZE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "imageaos.hpp"

// Function for pixel interpolation
template <typename T>
Pixel<T> interpolation(Pixel<T> c1, Pixel<T> c2, double frac) {
    Pixel<T> result;
    result.b = static_cast<int>(c1.b) + (static_cast<int>(c2.b)- static_cast<int>(c1.b)) * frac;
    result.r = static_cast<int>(c1.r) + (static_cast<int>(c2.r)- static_cast<int>(c1.r)) * frac;
    result.g = static_cast<int>(c1.g) + (static_cast<int>(c2.g)- static_cast<int>(c1.g)) * frac;
    return result;
}

template <typename T>
std::vector<Pixel<T> > resize(std::vector<Pixel<T> > pixels, Metadata metadata, std::vector<int> size){

    // Define the new dimensions
    int newWidth = size[0], newHeight = size[1];
    std::vector<Pixel<T> > newPixels(static_cast<size_t>(newWidth) * newHeight);

    Pixel<T> c1, c2;
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            int x_l = j * metadata.width / newWidth;
            int y_l = i * metadata.height / newHeight;
            int x_h = std::min(x_l + 1, metadata.width - 1);   // To avoid out-of-bounds access
            int y_h = std::min(y_l + 1, metadata.height - 1);  // To avoid out-of-bounds access

            double x_diff = j * (double)metadata.width / newWidth - x_l;
            double y_diff = i * (double)metadata.height / newHeight - y_l;

            // Perform interpolation
            c1 = interpolation<T>(pixels[static_cast<size_t>(y_l * metadata.width + x_l)], pixels[static_cast<size_t>(y_l * metadata.width + x_h)], x_diff);
            c2 = interpolation<T>(pixels[static_cast<size_t>(y_h * metadata.width + x_l)], pixels[static_cast<size_t>(y_h * metadata.width + x_h)], x_diff);
            newPixels[static_cast<size_t>(i * newWidth + j)] = interpolation<T>(c1, c2, y_diff);
        }
    }
    return newPixels;
}

#endif // RESIZE_HPP
