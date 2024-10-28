// compress.cpp
#include "compress.hpp"
#include <fstream>
#include <iostream>

//hay q delcarar color max???

// leer la file??

CCompress::CCompress(const std::string& outputFilePath) : outputFilePath(outputFilePath) {}

//{
// void CCompress::compress(const std::vector<Pixel>& imagePixels) {
    //std::ifstream input(params.input_file, std::ios::binary);
    //if (!input.is_open()) {
      //std::cerr << "Error opening file " << params.input_file << std::endl;
      //return;
    /}
//}

void getpixels(const inputFile) {
  for (std::size_t i = 0; i < dataSize; i++) {
    image[i].r = readBinary<T>(inputFile);
    data[i].g = readBinary<T>(inputFile);
    data[i].b = readBinary<T>(inputFile);
  }
  return data;
}
void CCompress::writeCompressedData(const std::vector<uint8_t>& compressedData) {
    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening output file: " << outputFilePath << std::endl;
        return;
    }

    outFile.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
    outFile.close();
}
