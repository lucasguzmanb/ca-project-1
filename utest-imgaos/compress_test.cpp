#include "common/binaryio.hpp"
#include "imgaos/compress.hpp"

#include <gtest/gtest.h>

namespace {
  template <typename T>
  void comparefiles(std::ifstream &inputFile, std::ifstream &outputFile) {
    }
  }




TEST(compressAOSTest, compress) {
  // Define test cases
  struct TestCase {
    std::filesystem::path inputFile;
    std::filesystem::path outputFile;
    int newMaxColorValue;
  };
  template <typename T>
  std::vector<TestCase> const testCases = {
    {.inputFile        = "input/deer-small.ppm",
     .outputFile       = "output/deer-small.ppm"},
    /*{.inputFile        = "input/deer-small.ppm",
     .outputFile       = "output/maxlevel/deer-small-255.ppm",
     .newMaxColorValue = 255  },
    {.inputFile        = "input/deer-small.ppm",
     .outputFile       = "output/maxlevel/deer-small-1000.ppm",
     .newMaxColorValue = 1000 },
    {.inputFile        = "input/deer-small.ppm",
     .outputFile       = "output/maxlevel/deer-small-65535.ppm",
     .newMaxColorValue = 65535},
    {.inputFile        = "input/lake-small.ppm",
     .outputFile       = "output/maxlevel/lake-small-255.ppm",
     .newMaxColorValue = 255  },
    {.inputFile        = "input/lake-small.ppm",
     .outputFile       = "output/maxlevel/lake-small-65535.ppm",
     .newMaxColorValue = 65535}*/
  };

  for (auto const & testCase : testCases) {
    //inputFile is the cppm file, we decompress it
    std::ifstream inputFile      = openInputFile(testCase.inputFile);
    Metadata const inputMetadata = obtainMetadata(inputFile);
    Metadata newMetadata         = inputMetadata;
    //newMetadata.maxColorValue    = testCase.newMaxColorValue;


    std::map<uint32_t, Pixel<T>> colorMap;

    // Read metadata (assuming it includes the number of colors)
    obtainMetadata(inputFile);
    int width = metadata.width;
    int height = metadata.height;
    int maxColorValue = metadata.maxColorValue;
    int numColors;
    inputFile >> numColors;
    inputFile.ignore(1, ' ');
    //int numColors = metadata.numColors;

    outputFile.write(reinterpret_cast<const char*>(&width), sizeof(int));
    outputFile.write(reinterpret_cast<const char*>(&height), sizeof(int));
    outputFile.write(reinterpret_cast<const char*>(&maxColorValue), sizeof(int));
    outputFile.write(reinterpret_cast<const char*>(&numColors), sizeof(int));

    // Read the color table
    for (int i = 0; i < numColors; ++i) {
      Pixel<T> pixel;
      inputFile.read(reinterpret_cast<char*>(&pixel.r), sizeof(T));
      inputFile.read(reinterpret_cast<char*>(&pixel.g), sizeof(T));
      inputFile.read(reinterpret_cast<char*>(&pixel.b), sizeof(T));
      colorMap[i] = pixel; // Map index to pixel
    }

    // Read the color table until pixel indices start
    uint32_t index = 0;
    while (true) {
      Pixel<T> pixel;
      // Try to read a color entry
      inputFile.read(reinterpret_cast<char*>(&pixel.r), sizeof(T));
      inputFile.read(reinterpret_cast<char*>(&pixel.g), sizeof(T));
      inputFile.read(reinterpret_cast<char*>(&pixel.b), sizeof(T));

      // Check if we reached the end of file or an invalid color entry
      if (inputFile.eof() || inputFile.fail()) {
        break;
      }

      // Map the color to the current index
      colorMap[index++] = pixel;
    }

    // Clear any input stream errors before reading pixel indices
    inputFile.clear();
    inputFile.seekg(inputFile.tellg()); // Reset file position indicator to continue reading

    // Read pixel indices and write decompressed pixel data to the output file
    while (!inputFile.eof()) {
      uint32_t colorIndex;

      // Determine the index size based on the size of the color map
      if (index <= 256) {  // 1 byte (8-bit index)
        uint8_t index8;
        inputFile.read(reinterpret_cast<char*>(&index8), sizeof(uint8_t));
        colorIndex = static_cast<uint32_t>(index8);
      } else if (index <= 65536) {  // 2 bytes (16-bit index)
        uint16_t index16;
        inputFile.read(reinterpret_cast<char*>(&index16), sizeof(uint16_t));
        colorIndex = static_cast<uint32_t>(index16);
      } else {  // 4 bytes (32-bit index)
        inputFile.read(reinterpret_cast<char*>(&colorIndex), sizeof(uint32_t));
      }

      // Stop if end of file or read failure occurs
      if (inputFile.eof() || inputFile.fail()) {
        break;
      }

      // Look up the pixel in the color map and write it to the output file
      if (colorMap.find(colorIndex) != colorMap.end()) {
        Pixel<T> pixel = colorMap[colorIndex];
        outputFile.write(reinterpret_cast<const char*>(&pixel.r), sizeof(T));
        outputFile.write(reinterpret_cast<const char*>(&pixel.g), sizeof(T));
        outputFile.write(reinterpret_cast<const char*>(&pixel.b), sizeof(T));
      }
    }
  }
}

