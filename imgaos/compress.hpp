#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "imageaos.hpp"
class CCompress {
  public:
  // Constructor
  CCompress(const inputFile);

  void CCompress::compress(const std::vector<Pixel<uint8_t>>(&inputPixels), maxColorValue);

  void getpixels(const inputFile);

  std:: map<(Pixel<uint16_>), uint16_> getcolors(const std::vector<Pixel<uint16_t>>(%inputPixels), maxColorValue, n_colors);

  void CCompress::writeCompressedData(outputFile, const std::map<Color, uint16_>& pixelMap, n_colors);

};

#endif //COMPRESS_HPP