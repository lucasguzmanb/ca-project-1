#include "common/binaryio.hpp"
#include "imgsoa/cutfreq.hpp"
#include "imgsoa/imagesoa.hpp"

#include <gtest/gtest.h>

namespace {
  template <typename T>
  void comparePixels(ImageSOA<T> const & pixels,
                     ImageSOA<T> const & expectedPixels) {
    ASSERT_EQ(pixels.r.size(), expectedPixels.r.size());
    ASSERT_EQ(pixels.g.size(), expectedPixels.g.size());
    ASSERT_EQ(pixels.b.size(), expectedPixels.b.size());
    for (size_t i = 0; i < pixels.r.size(); ++i) {
      EXPECT_NEAR(pixels.r[i], expectedPixels.r[i], 5);
      EXPECT_NEAR(pixels.g[i], expectedPixels.g[i], 5);
      EXPECT_NEAR(pixels.b[i], expectedPixels.b[i], 5);
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
TEST(cutfreqSOATest, ChangeColorsAndCompare) {
  std::vector<TestCase> const testCases = {
    {.inputFile        = "utest-imgsoa/input/cutfreq/prueba.ppm",
     .outputFile       = "utest-imgsoa/output/cutfreq/prueba_output.ppm",
     .NumberOfColors = 5  },
    {.inputFile        = "utest-imgsoa/input/cutfreq/deer-small.ppm",
     .outputFile       = "utest-imgsoa/output/cutfreq/black-image.ppm",
     .NumberOfColors = 162000 }
  };

  for (auto const & testCase : testCases) {
    std::ifstream inputFile      = openInputFile(testCase.inputFile);
    Metadata const inputMetadata = obtainMetadata(inputFile);

    constexpr int THRESHOLD = 255;
    bool const isInputUint8 = inputMetadata.maxColorValue <= THRESHOLD;

    // Load input image

    ImageSOA<uint8_t> inputPixels8;
    ImageSOA<uint16_t> inputPixels16;

    if (isInputUint8) {
      inputPixels8 = binaryToSOA<uint8_t>(inputFile, inputMetadata.width, inputMetadata.height);
    } else {
      inputPixels16 = binaryToSOA<uint16_t>(inputFile, inputMetadata.width, inputMetadata.height);
    }

    // Perform cutfreq operation

    ImageSOA<uint8_t> outputPixels8;
    ImageSOA<uint16_t> outputPixels16;
    if (isInputUint8) {
        outputPixels8 = inputPixels8;
        removeLFCsoa<uint8_t>(outputPixels8, testCase.NumberOfColors);
      } else {
        outputPixels16 = inputPixels16;
        removeLFCsoa<uint16_t>(outputPixels16, testCase.NumberOfColors);
      }


    // Load reference output image
    std::ifstream referenceFile      = openInputFile(testCase.outputFile);
    Metadata const referenceMetadata = obtainMetadata(referenceFile);

    bool const isOutputUint8 = referenceMetadata.maxColorValue <= THRESHOLD;

    ImageSOA<uint8_t> expectedPixels8;
    ImageSOA<uint16_t> expectedPixels16;

    if (isOutputUint8) {
      expectedPixels8 =
          binaryToSOA<uint8_t>(referenceFile, referenceMetadata.width, referenceMetadata.height);
    } else {
      expectedPixels16 =
          binaryToSOA<uint16_t>(referenceFile, referenceMetadata.width, referenceMetadata.height);
    }


    // Compare each pixel
    if (isOutputUint8) {
      comparePixels<uint8_t>(outputPixels8, expectedPixels8);
    } else {
      comparePixels<uint16_t>(outputPixels16, expectedPixels16);
    }
  }
}
