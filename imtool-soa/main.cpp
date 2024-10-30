/* SOA */

#include "../common/binaryio.hpp"
#include "../common/progargs.hpp"
#include "../imgsoa/imagesoa.hpp"
#include "../imgsoa/resize.hpp"
#include "common/info.hpp"
#include "imgsoa/maxlevel.hpp"

#include <fstream>
#include <iostream>
#include <span>
#include <variant>
#include <vector>

int main(int const argc, char * argv[]) {
  // check & parse arguments
  check_minimum_args(argc);
  std::span<char *> const args_view(argv + 1, static_cast<std::size_t>(argc - 1));
  std::vector<std::string> const args_vector = {args_view.begin(), args_view.end()};
  Arguments const args                       = checkAndParseArgs(args_vector);

  // open input file
  std::ifstream inputFile = openInputFile(args.input);

  Metadata const metadata = obtainMetadata(inputFile);
  Metadata newMetadata    = metadata;
  // check if file is in P6 format
  if (metadata.format != "P6") {
    std::cerr << "Error: file is not in .ppm format\n";
    exit(-1);
  }

  if (args.operation == "info") {
    info(args, metadata);
    return 0;
  }

  constexpr int THRESHOLD = 255;
  bool const isInputUint8 = metadata.maxColorValue <= THRESHOLD;

  // read binary data -> store using AOS (8 or 16 bits)
  std::variant<ImageSOA<uint8_t>, ImageSOA<uint16_t>> inputPixels;
  if (isInputUint8) {
    inputPixels = binaryToSOA<uint8_t>(inputFile, metadata.width, metadata.height);
  } else {
    inputPixels = binaryToSOA<uint16_t>(inputFile, metadata.width, metadata.height);
  }
  // perform requested operation (8 or 16 bits)
  std::variant<ImageSOA<uint8_t>, ImageSOA<uint16_t>> outputPixels;
  if (args.operation == "maxlevel") {
    if (isInputUint8) {
      outputPixels = maxlevel<uint8_t>(std::get<ImageSOA<uint8_t>>(inputPixels),
                                       metadata.maxColorValue, args.extra[0]);
    } else {
      outputPixels = maxlevel<uint16_t>(std::get<ImageSOA<uint16_t>>(inputPixels),
                                        metadata.maxColorValue, args.extra[0]);
    }
    newMetadata.maxColorValue = args.extra[0];
  } else if (args.operation == "resize") {
    if (isInputUint8) {
      outputPixels =
          resize<uint8_t>(std::get<ImageSOA<uint8_t>>(inputPixels), metadata, args.extra);
    } else {
      outputPixels =
          resize<uint16_t>(std::get<ImageSOA<uint16_t>>(inputPixels), metadata, args.extra);
    }
    newMetadata.width  = args.extra[0];
    newMetadata.height = args.extra[1];
  } else if (args.operation == "cutfreq") {
    outputPixels = inputPixels;
    if (isInputUint8) {
      std::cout << "cutfreq8\n";
    } else {
      std::cout << "cutfreq16\n";
    }
  } else if (args.operation == "compress") {
    if (isInputUint8) {
      std::cout << "compress8\n";
    } else {
      std::cout << "compress16\n";
    }
  } else {
    std::cerr << "Error: unknown operation\n";
    exit(-1);
  }

  /// open output file
  std::ofstream outputFile = openOutputFile(args.output);

  // write metadata
  writeMetadata(outputFile, newMetadata);

  // write binary data
  if (std::holds_alternative<ImageSOA<uint8_t>>(outputPixels)) {
    SOAToBinary<uint8_t>(outputFile, std::get<ImageSOA<uint8_t>>(outputPixels));
  } else if (std::holds_alternative<ImageSOA<uint16_t>>(outputPixels)) {
    SOAToBinary<uint16_t>(outputFile, std::get<ImageSOA<uint16_t>>(outputPixels));
  } else {
    std::cerr << "Error: unknown variant type\n";
    exit(-1);
  }

  inputFile.close();
  outputFile.close();

  return 0;
}