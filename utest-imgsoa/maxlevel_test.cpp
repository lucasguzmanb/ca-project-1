#include "common/binaryio.hpp"
#include "imgsoa/maxlevel.hpp"
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
      EXPECT_EQ(pixels.r[i], expectedPixels.r[i]);
      EXPECT_EQ(pixels.g[i], expectedPixels.g[i]);
      EXPECT_EQ(pixels.b[i], expectedPixels.b[i]);
    }
  }
}  // namespace

// Define test cases
struct TestCase {
    std::string inputFile;
    std::string outputFile;
    int newMaxColorValue;
};

// Test to change intensity level and compare with reference outputs
TEST(maxlevelSOATest, ChangeIntensityAndCompare) {
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

    ImageSOA<uint8_t> inputPixels8;
    ImageSOA<uint16_t> inputPixels16;

    if (isInputUint8) {
      inputPixels8 = binaryToSOA<uint8_t>(inputFile, inputMetadata.width, inputMetadata.height);
    } else {
      inputPixels16 = binaryToSOA<uint16_t>(inputFile, inputMetadata.width, inputMetadata.height);
    }

    // Perform maxlevel operation
    ImageSOA<uint8_t> outputPixels8;
    ImageSOA<uint16_t> outputPixels16;

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

    ImageSOA<uint8_t> expectedPixels8;
    ImageSOA<uint16_t> expectedPixels16;

    if (isOutputUint8) {
      expectedPixels8 =
          binaryToSOA<uint8_t>(referenceFile, referenceMetadata.width, referenceMetadata.height);
    } else {
      expectedPixels16 =
          binaryToSOA<uint16_t>(referenceFile, referenceMetadata.width, referenceMetadata.height);
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