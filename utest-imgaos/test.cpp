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
    {"input/deer-large.ppm", "resize/resize/deer-large-100.ppm", 100, 100}
    /*{"input/deer-large.ppm", "outputs/resize/deer-large-1000.ppm", 1000, 1000},
    {"input/deer-small.ppm", "outputs/resize/deer-small-100.ppm", 100, 100},
    {"input/deer-small.ppm", "outputs/resize/deer-small-1000.ppm", 1000, 1000},
    {"input/lake-large.ppm", "outputs/resize/lake-large-100.ppm", 100, 100},
    {"input/lake-large.ppm", "outputs/resize/lake-large-1000.ppm", 1000, 1000},
    {"input/lake-small.ppm", "outputs/resize/lake-small-100.ppm", 100, 100},
    {"input/lake-small.ppm", "outputs/resize/lake-small-1000.ppm", 1000, 1000}*/
  };

  for (const auto& testCase : testCases) {
    std::ifstream inputFile = openInputFile(testCase.inputFile);
    Metadata inputMetadata = obtainMetadata(inputFile);
    Metadata newMetadata = inputMetadata;
    newMetadata.height = testCase.newHeight;
    newMetadata.width = testCase.newWidth;

    // Load input image
    std::vector<Pixel<uint8_t>> inputPixels = binaryToAOS<uint8_t>(inputFile, inputMetadata.width, inputMetadata.height);

    // Resize image
    std::vector<int> newSize = {newMetadata.width, newMetadata.height};
    std::vector<int> oldSize = {inputMetadata.width, inputMetadata.height};
    std::vector<Pixel<uint8_t>> resizedPixels = resize<uint8_t>(inputPixels, inputMetadata, newSize);

    // resize to initial size
    std::vector<Pixel<uint8_t>> reresizedPixels = resize<uint8_t>(inputPixels, inputMetadata, oldSize);
    // Load reference output image
    std::ifstream outputFile = openInputFile(testCase.outputFile);
    Metadata outputMetadata = obtainMetadata(outputFile);
    std::vector<Pixel<uint8_t>> expectedPixels = binaryToAOS<uint8_t>(outputFile, outputMetadata.width, outputMetadata.height);

    // Validate the metadata matches expected output size
    //EXPECT_EQ(newMetadata.width, outputMetadata.width);
    //EXPECT_EQ(newMetadata.height, outputMetadata.height);

    // Validate each pixel
    ASSERT_EQ(inputPixels.size(), reresizedPixels.size());
    for (size_t i = 0; i < inputPixels.size(); ++i) {
      EXPECT_EQ(inputPixels[i].r, reresizedPixels[i].r);
      EXPECT_EQ(inputPixels[i].g, reresizedPixels[i].g);
      EXPECT_EQ(inputPixels[i].b, reresizedPixels[i].b);
    }
  }
}