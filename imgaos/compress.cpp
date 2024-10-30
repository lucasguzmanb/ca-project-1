// compress.cpp
#include "compress.hpp"
#include <fstream>
#include <iostream>
//otros includes??

CCompress::CCompress(const std::string& outputFilePath) : outputFilePath(outputFilePath) {}

//{
void CCompress::compress(const std::vector<Pixel,uint8_t>(inputPixels), metadata.maxColorValue, args.extra[0]) {
    //initialize values of width, height and colormax
    maxColorValue = metadata.maxColorValue;
    getpixels(inputPixels);
    std::map<Color, uint16_t>colorMap = getcolors(inputPixels, n_colors);
    write_data(outputFile, colorMap, maxColorValue);

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

std:: map<Color, uint16_> getcolors(const std::vector<Pixel,uint8_t>(inputPixels),metadata.maxColorValue, args.extra[0]) {
  std::map<Color, uint16_> colours;
  for (cons //iterate through colours input inputfile){
  	if (colours.find(Color(0, 0, 0)) != colours.end()) {
    	tablecolours[color] = n_colours + 1;
        n_colours++;
    }
  }
}


void CCompress::writeCompressedData(outputFile, const std::map<Color, uint16_>& colorMap, n_colors) {
	int limitvalue = 255;
    int limitvalue2 = 65535;
    for (coonst [color: colorMap]{
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
        	index = static_cast<uint8_t>(colorMap[i];
			binary::writeBinary(outputFile, index);
    	}else if ((n_colours >=limit1+1) && (n_colours <= limitvalue2)) {
        	uint16_t index =colorMap[i];
            binary::writeBinary(outputFile, index);
        }else{
          index = static_cast<uint32_t>(colorMap[i]);
          binary::writeBinary(outputFile, index);
        }
    }
}

