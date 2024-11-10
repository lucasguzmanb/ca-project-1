#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "imageaos.hpp"
#include <map>


template <typename T>
std::vector<Pixel<T>> compress(const std::vector<Pixel<uint8_t>> inputPixels, Metadata metadata) {
    //initialize values
    int const maxColorValue = metadata.maxColorValue;
    std:: map<Pixel<T>, T> pixelMap = getcolors(inputPixels, maxColorValue, n_colors);
    std::vector<Pixel<T>> outputPixels = write_data(outputFile, pixelMap, maxColorValue);

}

template <typename T>
std:: map<Pixel<T>, T> getcolors(std::vector<Pixel<T>>inputPixels), maxColorValue, n_colors) {
  std::map<Pixel, uint16_> Pixel_map;
   for (const auto& pixel : pixels) {
        // Check if the color is already in the map
        if (Pixel_map.find(pixel) == Pixel_map.end()) {
            // Assign a new index to this color
            uint16_t index = static_cast<uint16_t>(Pixel_map.size());
            Pixel_map[pixel] = index;
        }
   }
}

template <typename T>
void writeCompressedData(outputFile, const std::map<Pixel<T>, uint16_t>& pixelMap, n_colors) {
  int const limitvalue = 255;
  int const limitvalue2 = 65535;
    for (const [pixel: pixelMap]{
    	if(maxColorValue < limitvalue) {
    	    const r = static_cast<uint8_t>(color.r);
            const g = static_cast<uint8_t>(color.g);
            const b = static_cast<uint8_t>(color.b);
            binary::writeBinary(outputFile, r);
            binary::writeBinary(outputFile, g);
            binary::writeBinary(outputFile, b);
        }else{
            binary::writeBinary(outputFile, color.r);
            binary::writeBinary(outputFile, color.g);
            binary::writeBinary(outputFile, color.b);
        }
    }
    for (std::size_t i = 0; i < pixels.size; i++) {
    	if (n_colors <= limitvalue) {
        	index = static_cast<uint8_t>(Pixel_map[i];
			binary::writeBinary(outputFile, index);
    	}else if ((n_colours >=limit1+1) && (n_colours <= limitvalue2)) {
        	uint16_t index =Pixel_map[i];
            binary::writeBinary(outputFile, index);
        }else{
          index = static_cast<uint32_t>(Pixel_map[i]);
          binary::writeBinary(outputFile, index);
        }
    }
}











};

#endif //COMPRESS_HPP