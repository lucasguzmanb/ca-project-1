/* SOA */

#include "common/binaryio.hpp"
#include "common/info.hpp"
#include "common/progargs.hpp"
#include "imgsoa/compress.hpp"
#include "imgsoa/cutfreq.hpp"
#include "imgsoa/imagesoa.hpp"
#include "imgsoa/maxlevel.hpp"
#include "imgsoa/resize.hpp"

#include <fstream>
#include <iostream>
#include <span>
#include <vector>

int main(int const argc, char * argv[]) { // NOLINT(readability-function-size)
  // check & parse arguments
  check_minimum_args(argc);
  std::span<char *> const args_view(argv + 1, static_cast<std::size_t>(argc - 1));
  std::vector<std::string> const args_vector = {args_view.begin(), args_view.end()};
  Arguments const args                       = checkAndParseArgs(args_vector);

  // open input file
  std::ifstream inputFile = openInputFile(args.input);

  Metadata const metadata = obtainMetadata(inputFile);
  Metadata newMetadata    = metadata;
  checkFileFormat(metadata.format);

  if (args.operation == "info") {
    info(args, metadata);
    return 0;
  }

  constexpr int THRESHOLD = 255;
  bool const isInputUint8 = metadata.maxColorValue <= THRESHOLD;

  // read binary data -> store using AOS (8 or 16 bits)
  ImageSOA<uint8_t> inputPixels8;
  ImageSOA<uint16_t> inputPixels16;
  if (isInputUint8) {
    inputPixels8 = binaryToSOA<uint8_t>(inputFile, metadata.width, metadata.height);
  } else {
    inputPixels16 = binaryToSOA<uint16_t>(inputFile, metadata.width, metadata.height);
  }
  // perform requested operation (8 or 16 bits)
  ImageSOA<uint8_t> outputPixels8;
  ImageSOA<uint16_t> outputPixels16;

  /// open output file
  std::ofstream outputFile = openOutputFile(args.output);
  if (args.operation == "maxlevel") {
    if (isInputUint8) {
      if (args.extra[0] <= THRESHOLD) {
        outputPixels8 =
            maxlevel<uint8_t, uint8_t>(inputPixels8, metadata.maxColorValue, args.extra[0]);
      } else {
        outputPixels16 =
            maxlevel<uint8_t, uint16_t>(inputPixels8, metadata.maxColorValue, args.extra[0]);
      }
    } else {
      if (args.extra[0] <= THRESHOLD) {
        outputPixels8 =
            maxlevel<uint16_t, uint8_t>(inputPixels16, metadata.maxColorValue, args.extra[0]);
      } else {
        outputPixels16 =
            maxlevel<uint16_t, uint16_t>(inputPixels16, metadata.maxColorValue, args.extra[0]);
      }
    }
    newMetadata.maxColorValue = args.extra[0];
  } else if (args.operation == "resize") {
    if (isInputUint8) {
      outputPixels8 =
          resize<uint8_t>(inputPixels8, metadata, args.extra);
    } else {
      outputPixels16 =
          resize<uint16_t>(inputPixels16, metadata, args.extra);
    }
    newMetadata.width  = args.extra[0];
    newMetadata.height = args.extra[1];
  } else if (args.operation == "cutfreq") {
    if (isInputUint8) {
      outputPixels8 = inputPixels8;
      removeLFCsoa(outputPixels8, args.extra[0]);
    } else {
      outputPixels16 = inputPixels16;
      removeLFCsoa(outputPixels16, args.extra[0]);
    }
  } else if (args.operation == "compress") {
    newMetadata.format = "C6";
    if (isInputUint8) {
      compress(inputPixels8, outputFile, newMetadata);
    } else {
      compress(inputPixels16, outputFile, newMetadata);
    }
    return 0;
  } else {
    std::cerr << "Error: unknown operation\n";
    return -1;
  }

  // write metadata
  writeMetadata(outputFile, newMetadata);

  // write binary data
  if (newMetadata.maxColorValue <= THRESHOLD) {
    SOAToBinary<uint8_t>(outputFile, outputPixels8);
  } else {
    SOAToBinary<uint16_t>(outputFile, outputPixels16);
  }

  inputFile.close();
  outputFile.close();

  return 0;
}