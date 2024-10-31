/* Support for image processing with SOA */
#include "imagesoa.hpp"

#include <iostream>
#include <variant>

void writeBinaryData(std::ofstream & outputFile,
                     std::variant<ImageSOA<uint8_t>, ImageSOA<uint16_t>> const & outputPixels) {
  if (std::holds_alternative<ImageSOA<uint8_t>>(outputPixels)) {
    SOAToBinary<uint8_t>(outputFile, std::get<ImageSOA<uint8_t>>(outputPixels));
  } else if (std::holds_alternative<ImageSOA<uint16_t>>(outputPixels)) {
    SOAToBinary<uint16_t>(outputFile, std::get<ImageSOA<uint16_t>>(outputPixels));
  } else {
    std::cerr << "Error: unknown variant type\n";
    exit(-1);
  }
}