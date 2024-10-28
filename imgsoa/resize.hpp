#ifndef RESIZE_HPP
#define RESIZE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "imagesoa.hpp"

// Function for pixel interpolation
template <typename T>
T interpolation(T color1, T color2, double frac) {
    return static_cast<T> (color1 + (color2 - color1) * frac);
}

template <typename T>
ImageSOA<T> resize(ImageSOA<T> pixels, Metadata metadata, std::vector<int> size){

    // Define the new dimensions
    int newWidth = size[0], newHeight = size[1];
    ImageSOA<T> newPixels;
    newPixels.r.resize(static_cast<size_t>(newWidth * newHeight));
    newPixels.g.resize(static_cast<size_t>(newWidth * newHeight));
    newPixels.b.resize(static_cast<size_t>(newWidth * newHeight));


    T c1, c2;
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            int x_l = j * metadata.width / newWidth;
            int y_l = i * metadata.height / newHeight;
            int x_h = std::min(x_l + 1, metadata.width - 1);   // To avoid out-of-bounds access
            int y_h = std::min(y_l + 1, metadata.height - 1);  // To avoid out-of-bounds access

            double x_diff = j * (double)metadata.width / newWidth - x_l;
            double y_diff = i * (double)metadata.height / newHeight - y_l;

            c1 = interpolation<T>(pixels.r[static_cast<size_t>(y_l * metadata.width + x_l)], pixels.r[static_cast<size_t>(y_l * metadata.width + x_h)], x_diff);
            c2 = interpolation<T>(pixels.r[static_cast<size_t>(y_h * metadata.width + x_l)], pixels.r[static_cast<size_t>(y_h * metadata.width + x_h)], x_diff);
            newPixels.r[static_cast<size_t>(i * newWidth + j)] = interpolation<T>(c1, c2, y_diff);

            c1 = interpolation<T>(pixels.g[static_cast<size_t>(y_l * metadata.width + x_l)], pixels.g[static_cast<size_t>(y_l * metadata.width + x_h)], x_diff);
            c2 = interpolation<T>(pixels.g[static_cast<size_t>(y_h * metadata.width + x_l)], pixels.g[static_cast<size_t>(y_h * metadata.width + x_h)], x_diff);
            newPixels.g[static_cast<size_t>(i * newWidth + j)] = interpolation<T>(c1, c2, y_diff);

            c1 = interpolation<T>(pixels.b[static_cast<size_t>(y_l * metadata.width + x_l)], pixels.b[static_cast<size_t>(y_l * metadata.width + x_h)], x_diff);
            c2 = interpolation<T>(pixels.b[static_cast<size_t>(y_h * metadata.width + x_l)], pixels.b[static_cast<size_t>(y_h * metadata.width + x_h)], x_diff);
            newPixels.b[static_cast<size_t>(i * newWidth + j)] = interpolation<T>(c1, c2, y_diff);
            
        }
    }
    return newPixels;
}

#endif // RESIZE_HPP
