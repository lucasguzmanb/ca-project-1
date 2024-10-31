#include "imageaos.hpp"

void writeBinaryData(
    std::ofstream & outputFile,
    std::variant<std::vector<Pixel<uint8_t>>, std::vector<Pixel<uint16_t>>> const & outputPixels) {
  if (std::holds_alternative<std::vector<Pixel<uint8_t>>>(outputPixels)) {
    AOSToBinary<uint8_t>(outputFile, std::get<std::vector<Pixel<uint8_t>>>(outputPixels));
  } else if (std::holds_alternative<std::vector<Pixel<uint16_t>>>(outputPixels)) {
    AOSToBinary<uint16_t>(outputFile, std::get<std::vector<Pixel<uint16_t>>>(outputPixels));
  } else {
    std::cerr << "Error: unknown variant type\n";
    exit(-1);
  }
}