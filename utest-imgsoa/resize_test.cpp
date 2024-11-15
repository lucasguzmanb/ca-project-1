// utest-imgsoa/test_resize.cpp

#include "../common/binaryio.hpp"
#include "../imgsoa/resize.hpp"

#include <gtest/gtest.h>

// Test to resize images and compare with reference outputs
TEST(utest_imgsoa, ResizeAndCompare) {
  // Define test cases
  struct TestCase {
      std::string inputFile;
      std::string outputFile;
      int newWidth;
      int newHeight;
  };

  std::vector<TestCase> const testCases = {
    {.inputFile  = "input/deer-large.ppm",
     .outputFile = "output/resize/deer-large-100.ppm",
     .newWidth   = 100,
     .newHeight  = 100 },
    {.inputFile  = "input/deer-large.ppm",
     .outputFile = "output/resize/deer-large-1000.ppm",
     .newWidth   = 1000,
     .newHeight  = 1000},
    {.inputFile  = "input/deer-small.ppm",
     .outputFile = "output/resize/deer-small-100.ppm",
     .newWidth   = 100,
     .newHeight  = 100 },
    {.inputFile  = "input/deer-small.ppm",
     .outputFile = "output/resize/deer-small-1000.ppm",
     .newWidth   = 1000,
     .newHeight  = 1000},
    {.inputFile  = "input/lake-large.ppm",
     .outputFile = "output/resize/lake-large-100.ppm",
     .newWidth   = 100,
     .newHeight  = 100 },
    {.inputFile  = "input/lake-large.ppm",
     .outputFile = "output/resize/lake-large-1000.ppm",
     .newWidth   = 1000,
     .newHeight  = 1000},
    {.inputFile  = "input/lake-small.ppm",
     .outputFile = "output/resize/lake-small-100.ppm",
     .newWidth   = 100,
     .newHeight  = 100 },
    {.inputFile  = "input/lake-small.ppm",
     .outputFile = "output/resize/lake-small-1000.ppm",
     .newWidth   = 1000,
     .newHeight  = 1000}
  };

  for (auto const & testCase : testCases) {
    std::ifstream inputFile = openInputFile(testCase.inputFile);
    Metadata const inputMetadata  = obtainMetadata(inputFile);
    Metadata outputMetadata = inputMetadata;
    outputMetadata.height   = testCase.newHeight;
    outputMetadata.width    = testCase.newWidth;

    // Load input image
    ImageSOA<uint8_t> const inputPixels =
        binaryToSOA<uint8_t>(inputFile, inputMetadata.width, inputMetadata.height);

    // Resize image
    std::vector<int> newSize = {testCase.newWidth, testCase.newHeight};
    ImageSOA<uint8_t> resizedPixels =
        resize<uint8_t>(inputPixels, inputMetadata, newSize);

    // Load reference output image
    std::ifstream outputFile = openInputFile(testCase.outputFile);
    outputMetadata           = obtainMetadata(outputFile);
    ImageSOA<uint8_t> expectedPixels =
        binaryToSOA<uint8_t>(outputFile, outputMetadata.width, outputMetadata.height);

    // Validate the metadata matches expected output size
    EXPECT_EQ(newSize[0], outputMetadata.width);
    EXPECT_EQ(newSize[1], outputMetadata.height);

    // Validate each pixel
    ASSERT_EQ(resizedPixels.size(), expectedPixels.size());
    for (size_t i = 0; i < resizedPixels.r.size(); ++i) {
      EXPECT_EQ(resizedPixels.r[i], expectedPixels.r[i]);
      EXPECT_EQ(resizedPixels.g[i], expectedPixels.g[i]);
      EXPECT_EQ(resizedPixels.b[i], expectedPixels.b[i]);
    }
  }
}