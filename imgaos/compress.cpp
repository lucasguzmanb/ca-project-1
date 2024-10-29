// compress.cpp
#include "compress.hpp"
#include <fstream>
#include <iostream>

//hay q delcarar color max???

// leer la file??

CCompress::CCompress(const std::string& outputFilePath) : outputFilePath(outputFilePath) {}

//{
void CCompress::compress(const std::vector<Pixel<uint8_t>(inputPixels),metadata.maxColorValue, args.extra[0] {
    //initialize values of width, height and colormax
    maxColorValue = metadata.maxColorValue;
    int width, height, maxColorValue = 0;
    openin

//}

void getpixels(const inputFile) {

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


void CCompress::writeCompressedData(const std::vector<uint8_t>& compressedData) {

    outFile.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
    outFile.close();
}
