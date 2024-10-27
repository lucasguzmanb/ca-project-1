// compress.cpp
#include "compress.hpp"
#include <fstream>
#include <iostream>

// Constructor implementation
CCompress::CCompress(const std::string& outputFilePath) : outputFilePath(outputFilePath) {}

void CCompress::compress(const std::vector<Pixel>& imagePixels) {
    // Convert the image pixels to a format suitable for compression
    std::vector<uint8_t> rawData;
    for (const auto& pixel : imagePixels) {
        rawData.push_back(pixel.red);
        rawData.push_back(pixel.green);
        rawData.push_back(pixel.blue);
    }

    // Compress the raw data using zlib
    uLongf compressedSize = compressBound(rawData.size());
    std::vector<uint8_t> compressedData(compressedSize);

    if (compress(compressedData.data(), &compressedSize, rawData.data(), rawData.size()) != Z_OK) {
        std::cerr << "Compression failed!" << std::endl;
        return;
    }

    compressedData.resize(compressedSize); // Resize to actual compressed size
    writeCompressedData(compressedData);
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

