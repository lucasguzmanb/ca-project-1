#include "common/binaryio.hpp"
#include "imgaos/cutfreq.hpp"

#include <gtest/gtest.h>

namespace {
  template <typename T>
  void comparePixels(std::vector<Pixel<T>> const & pixels,
                     std::vector<Pixel<T>> const & expectedPixels) {
    ASSERT_EQ(pixels.size(), expectedPixels.size());
    for (size_t i = 0; i < pixels.size(); ++i) {
      EXPECT_NEAR(pixels[i].r, expectedPixels[i].r, 5);
      EXPECT_NEAR(pixels[i].g, expectedPixels[i].g, 5);
      EXPECT_NEAR(pixels[i].b, expectedPixels[i].b, 5);
    }
  }
}  // namespace

// Define test cases
struct TestCase {
    std::filesystem::path inputFile;
    std::filesystem::path outputFile;
    int NumberOfColors;
};

// Test to change intensity level and compare with reference outputs
TEST(cutfreqAOSTest, ChangeColorsAndCompare) {
  std::vector<TestCase> const testCases = {
    {.inputFile        = "utest-imgaos/input/cutfreq/prueba.ppm",
     .outputFile       = "utest-imgaos/output/cutfreq/prueba_output.ppm",
     .NumberOfColors = 5  },
    {.inputFile        = "utest-imgaos/input/cutfreq/deer-small.ppm",
     .outputFile       = "utest-imgaos/output/cutfreq/black-image.ppm",
     .NumberOfColors = 162000 }
  };
  for (auto const & testCase : testCases) {
    std::ifstream inputFile      = openInputFile(testCase.inputFile);
    Metadata const inputMetadata = obtainMetadata(inputFile);

    constexpr int THRESHOLD = 255;
    bool const isInputUint8 = inputMetadata.maxColorValue <= THRESHOLD;

    // Load input image
    std::vector<Pixel<uint8_t>> inputPixels8;
    std::vector<Pixel<uint16_t>> inputPixels16;

    if (isInputUint8) {
      inputPixels8 = binaryToAOS<uint8_t>(inputFile, inputMetadata.width, inputMetadata.height);
    } else {
      inputPixels16 = binaryToAOS<uint16_t>(inputFile, inputMetadata.width, inputMetadata.height);
    }

    // Perform maxlevel operation
    std::vector<Pixel<uint8_t>> outputPixels8=inputPixels8;
    std::vector<Pixel<uint16_t>> outputPixels16=inputPixels16;

    if (isInputUint8) {
      removeLFCaos<uint8_t>(outputPixels8, testCase.NumberOfColors);
    } else{
      removeLFCaos<uint16_t>(outputPixels16, testCase.NumberOfColors);
    }


    // Load reference output image
    std::ifstream referenceFile      = openInputFile(testCase.outputFile);
    Metadata const referenceMetadata = obtainMetadata(referenceFile);

    bool const isOutputUint8 = referenceMetadata.maxColorValue <= THRESHOLD;

    std::vector<Pixel<uint8_t>> expectedPixels8;
    std::vector<Pixel<uint16_t>> expectedPixels16;

    if (isOutputUint8) {
      expectedPixels8 =
          binaryToAOS<uint8_t>(referenceFile, referenceMetadata.width, referenceMetadata.height);
    } else {
      expectedPixels16 =
          binaryToAOS<uint16_t>(referenceFile, referenceMetadata.width, referenceMetadata.height);
    }


    // Compare each pixel
    if (isOutputUint8) {
      comparePixels<uint8_t>(outputPixels8, expectedPixels8);
    } else {
      comparePixels<uint16_t>(outputPixels16, expectedPixels16);
    }
  }
}
