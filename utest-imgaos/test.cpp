// utest-imgaos/test_resize.cpp

#include <gtest/gtest.h>
#include "../imgaos/resize.hpp"
#include "../common/binaryio.hpp"


// Test to resize images and compare with reference outputs
TEST(utest_imgaos, ResizeAndCompare) {
  // Define test cases
  struct TestCase {
      std::string inputFile;
      std::string outputFile;
      int newWidth;
      int newHeight;
  };

  std::vector<TestCase> testCases = {
    {"input/deer-large.ppm", "outputs/resize/deer-large-100.ppm", 100, 100},
    {"input/deer-large.ppm", "outputs/resize/deer-large-1000.ppm", 1000, 1000},
    {"input/deer-small.ppm", "outputs/resize/deer-small-100.ppm", 100, 100},
    {"input/deer-small.ppm", "outputs/resize/deer-small-1000.ppm", 1000, 1000},
    {"input/lake-large.ppm", "outputs/resize/lake-large-100.ppm", 100, 100},
    {"input/lake-large.ppm", "outputs/resize/lake-large-1000.ppm", 1000, 1000},
    {"input/lake-small.ppm", "outputs/resize/lake-small-100.ppm", 100, 100},
    {"input/lake-small.ppm", "outputs/resize/lake-small-1000.ppm", 1000, 1000}
  };

  for (const auto& testCase : testCases) {
    std::ifstream inputFile = openInputFile(testCase.inputFile);
    Metadata inputMetadata = obtainMetadata(inputFile);
    Metadata outputMetadata = inputMetadata;
    outputMetadata.height = testCase.newHeight;
    outputMetadata.width = testCase.newWidth;

    // Load input image
    std::vector<Pixel<uint8_t>> inputPixels = binaryToAOS<uint8_t>(inputFile, inputMetadata.width, inputMetadata.height);

    // Resize image
    std::vector<int> newSize = {testCase.newWidth, testCase.newHeight};
    std::vector<Pixel<uint8_t>> resizedPixels = resize<uint8_t>(inputPixels, inputMetadata, newSize);

    // Load reference output image
    std::ifstream outputFile = openInputFile(testCase.outputFile);
    Metadata outputMetadata = obtainMetadata(outputFile);
    std::vector<Pixel<uint8_t>> expectedPixels = binaryToAOS<uint8_t>(outputFile, outputMetadata.width, outputMetadata.height);

    // Validate the metadata matches expected output size
    EXPECT_EQ(newSize[0], outputMetadata.width);
    EXPECT_EQ(newSize[1], outputMetadata.height);

    // Validate each pixel
    ASSERT_EQ(resizedPixels.size(), expectedPixels.size());
    for (size_t i = 0; i < resizedPixels.size(); ++i) {
      EXPECT_EQ(resizedPixels[i].r, expectedPixels[i].r);
      EXPECT_EQ(resizedPixels[i].g, expectedPixels[i].g);
      EXPECT_EQ(resizedPixels[i].b, expectedPixels[i].b);
    }
  }
}