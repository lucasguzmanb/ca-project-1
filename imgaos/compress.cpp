// compress.cpp
#include "compress.hpp"
#include <fstream>
#include <iostream>

// Constructor implementation
Compressor::Compressor(const ImageAOS& img) : image(img) {}

// Compression function implementation
void Compressor::compress(const std::string& outputFile) {
    std::unordered_map<Pixel, int, PixelHash> colorMap; // Hash map to store unique colors
    std::vector<int> compressedData; // Vector to store indices of colors

    // Create a color map
    for (const auto& pixel : image.pixels) {
        if (colorMap.find(pixel) == colorMap.end()) {
            colorMap[pixel] = colorMap.size(); // Assign a new index
        }
        compressedData.push_back(colorMap[pixel]); // Store the index
    }

    // Write the compressed data to a file
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening output file!" << std::endl;
        return;
    }

    // Write the size of the color map
    int colorMapSize = colorMap.size();
    outFile.write(reinterpret_cast<char*>(&colorMapSize), sizeof(colorMapSize));

    // Write the color map
    for (const auto& entry : colorMap) {
        outFile.write(reinterpret_cast<const char*>(&entry.first), sizeof(Pixel));
    }

    // Write the compressed data
    for (const auto& index : compressedData) {
        outFile.write(reinterpret_cast<char*>(&index), sizeof(index));
    }

    outFile.close();
}


