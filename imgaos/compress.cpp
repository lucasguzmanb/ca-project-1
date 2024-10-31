// compress.cpp
#include "compress.hpp"
#include <fstream>
#include <iostream>
//otros includes??

CCompress::CCompress(const std::string& outputFilePath) : outputFilePath(outputFilePath) {}

//{
void CCompress::compress(const std::vector<Pixel<uint8_t>>(&inputPixels), maxColorValue) {
    //initialize values of width, height and colormax
    maxColorValue = metadata.maxColorValue;
    getpixels(&inputPixels);
    std:: map<(Pixel<uint16_>), uint16_> pixelMap = getcolors(inputPixels, n_colors);
    write_data(outputFile, pixelMap, maxColorValue);

}

void getpixels(const inputFile){

  int const limitvalue = 255;
  for (std::size_t i = 0; i < pixels.size; i++) {
    if (maxColorValue < limitvalue) {
    	outputFile[i].r = binary::readBinary<uint8_t>(inputFile);
    	outputFile[i].g = binary::readBinary<uint8_t>(inputFile);
    	outputFile[i].b = binary::readBinary<uint8_t>(inputFile);
    }else{
   		outputFile[i].r = binary::readBinary<uint16_t>(inputFile);
        outputFile[i].g = binary::readBinary<uint16_t>(inputFile);
        outputFile[i].b = binary::readBinary<uint16_t>(inputFile);
  	}
  return data;
  }
}

std:: map<(Pixel<uint16_>), uint16_> getcolors(const std::vector<Pixel<uint8_t>>(%inputPixels),maxColorValue, n_colors) {
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


void CCompress::writeCompressedData(outputFile, const std::map<Color, uint16_>& pixelMap, n_colors) {
	int limitvalue = 255;
    int limitvalue2 = 65535;
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
    	if (n_colours <= limitvalue) {
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

