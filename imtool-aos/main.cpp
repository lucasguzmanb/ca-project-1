/* AOS */
#include "common/binaryio.hpp"
#include "common/progargs.hpp"
#include "imgaos/cutfreq.hpp"
#include "imgaos/imageaos.hpp"
#include "imgaos/maxlevel.hpp"
#include "imgaos/resize.hpp"

#include <fstream>
#include <iostream>
#include <span>
#include <vector>

int main(int const argc, char * argv[]) {
  // check & parse arguments
  check_minimum_args(argc);
  std::span<char *> const args_view(argv + 1, static_cast<std::size_t>(argc - 1));
  std::vector<std::string> const args_vector = {args_view.begin(), args_view.end()};
  Arguments const args                       = parse_args(args_vector);
  check_args(args);
  print_arguments(args);

  // open input file
  std::ifstream inputFile(args.input, std::ios::binary);
  // check if file exists or can be opened
  if (!inputFile.is_open()) {
    std::cerr << "Error: can't open file " << args.input << '\n';
    exit(-1);
  }

  Metadata metadata = obtainMetadata(inputFile);
  // check if file is in P6 format
  if (metadata.format != "P6") {
    std::cerr << "Error: file is not in .ppm format\n";
    exit(-1);
  }

  constexpr int THRESHOLD = 255;
  bool const isInputUint8 = metadata.maxColorValue <= THRESHOLD;

  // read binary data -> store using AOS (8 or 16 bits)
  std::variant<std::vector<Pixel<uint8_t>>, std::vector<Pixel<uint16_t>>> inputPixels;
  if (isInputUint8) {
    inputPixels = binaryToAOS<uint8_t>(inputFile, metadata.width, metadata.height);
  } else {
    inputPixels = binaryToAOS<uint16_t>(inputFile, metadata.width, metadata.height);
  }
  // perform requested operation (8 or 16 bits)
  std::variant<std::vector<Pixel<uint8_t>>, std::vector<Pixel<uint16_t>>> outputPixels;

  if (args.operation == "maxlevel") {
    if (isInputUint8) {
      outputPixels = maxlevel<uint8_t>(std::get<std::vector<Pixel<uint8_t>>>(inputPixels),
                                       metadata.maxColorValue, args.extra[0]);
    } else {
      outputPixels = maxlevel<uint16_t>(std::get<std::vector<Pixel<uint16_t>>>(inputPixels),
                                        metadata.width, metadata.height);
    }
  } else if (args.operation == "resize") {
    if (isInputUint8) {
      outputPixels =
          resize<uint8_t>(std::get<std::vector<Pixel<uint8_t>>>(inputPixels), metadata, args.extra);
    } else if (args.operation == "cutfreq") {
      outputPixels = inputPixels;
      if (isInputUint8) {
        removeLFCaos<uint8_t>(std::get<std::vector<Pixel<uint8_t>>>(outputPixels), args.extra[0]);
      } else {
        removeLFCaos<uint16_t>(std::get<std::vector<Pixel<uint16_t>>>(outputPixels), args.extra[0]);
      }
    }
  }else {
      outputPixels = resize<uint16_t>(std::get<std::vector<Pixel<uint16_t>>>(inputPixels), metadata,
                                      args.extra);
    }
    metadata.width  = args.extra[0];
    metadata.height = args.extra[1];
  } else {
    std::cerr << "Error: unknown operation\n";
    exit(-1);
  }

  // open output file
  std::ofstream outputFile(args.output, std::ios::binary);
  // check if file can be created
  if (!outputFile.is_open()) {
    std::cerr << "Error: can't create file " << args.output << '\n';
    exit(-1);
  }
  // write metadata
  outputFile << "P6" << '\n'
             << metadata.width << ' ' << metadata.height << '\n'
             << metadata.maxColorValue << '\n';

  // write binary data
  if (std::holds_alternative<std::vector<Pixel<uint8_t>>>(outputPixels)) {
    AOSToBinary<uint8_t>(outputFile, std::get<std::vector<Pixel<uint8_t>>>(outputPixels),
                         metadata.width, metadata.height);
  } else if (std::holds_alternative<std::vector<Pixel<uint16_t>>>(outputPixels)) {
    AOSToBinary<uint16_t>(outputFile, std::get<std::vector<Pixel<uint16_t>>>(outputPixels),
                          metadata.width, metadata.height);
  } else {
    std::cerr << "Error: unknown variant type\n";
  }

  inputFile.close();
  outputFile.close();

  return 0;
}