#include "common/binaryio.hpp"
#include "imgaos/maxlevel.hpp"

#include <gtest/gtest.h>

namespace {
  template <typename T>
  void comparePixels(std::vector<Pixel<T>> const & pixels,
                     std::vector<Pixel<T>> const & expectedPixels) {
    ASSERT_EQ(pixels.size(), expectedPixels.size());
    for (size_t i = 0; i < pixels.size(); ++i) {
      EXPECT_EQ(pixels[i].r, expectedPixels[i].r);
      EXPECT_EQ(pixels[i].g, expectedPixels[i].g);
      EXPECT_EQ(pixels[i].b, expectedPixels[i].b);
    }
  }
}  // namespace

// Define test cases
struct TestCase {
    std::filesystem::path inputFile;
    std::filesystem::path outputFile;
    int newMaxColorValue;
};

// Test to change intensity level and compare with reference outputs
TEST(maxlevelAOSTest, ChangeIntensityAndCompare) {
  std::vector<TestCase> const testCases = {
    {.inputFile        = "input/deer-small.ppm",
     .outputFile       = "output/maxlevel/deer-small-100.ppm",
     .newMaxColorValue = 100  },
    {.inputFile        = "input/deer-small.ppm",
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
     .newMaxColorValue = 65535}
  };

  for (auto const & testCase : testCases) {
    std::ifstream inputFile      = openInputFile(testCase.inputFile);
    Metadata const inputMetadata = obtainMetadata(inputFile);
    Metadata newMetadata         = inputMetadata;
    newMetadata.maxColorValue    = testCase.newMaxColorValue;

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
    std::vector<Pixel<uint8_t>> outputPixels8;
    std::vector<Pixel<uint16_t>> outputPixels16;

    if (isInputUint8) {
      if (testCase.newMaxColorValue <= THRESHOLD) {
        outputPixels8 = maxlevel<uint8_t, uint8_t>(inputPixels8, inputMetadata.maxColorValue,
                                                   testCase.newMaxColorValue);
      } else {
        outputPixels16 = maxlevel<uint8_t, uint16_t>(inputPixels8, inputMetadata.maxColorValue,
                                                     testCase.newMaxColorValue);
      }
    } else {
      if (testCase.newMaxColorValue <= THRESHOLD) {
        outputPixels8 = maxlevel<uint16_t, uint8_t>(inputPixels16, inputMetadata.maxColorValue,
                                                    testCase.newMaxColorValue);
      } else {
        outputPixels16 = maxlevel<uint16_t, uint16_t>(inputPixels16, inputMetadata.maxColorValue,
                                                      testCase.newMaxColorValue);
      }
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

    // Compare metadata
    EXPECT_EQ(newMetadata.maxColorValue, referenceMetadata.maxColorValue);

    // Compare each pixel
    if (isOutputUint8) {
      comparePixels<uint8_t>(outputPixels8, expectedPixels8);
    } else {
      comparePixels<uint16_t>(outputPixels16, expectedPixels16);
    }
  }
}