#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "imageaos.hpp"
class CCompress {
  public:
  // Constructor
  CCompress(const std::string& outputFilePath);

  // Method to compress the image
  void compress(const std::vector<Pixel>& imagePixels);

  private:
  std::string outputFilePath; // Path to the output compressed file

  // Helper method to write the compressed data to file
  void writeCompressedData(const std::vector<uint8_t>& compressedData);
};

#endif // CCOMPRESS_HPP

#endif //COMPRESS_HPP