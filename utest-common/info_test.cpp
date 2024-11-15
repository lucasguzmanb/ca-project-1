#include "common/binaryio.hpp"

#include <complex>
#include <filesystem>
#include <gtest/gtest.h>
#include <sstream>

namespace {
  void compareMetadata(Metadata const & expected, Metadata const & actual) {
    EXPECT_EQ(expected.format, actual.format);
    EXPECT_EQ(expected.width, actual.width);
    EXPECT_EQ(expected.height, actual.height);
    EXPECT_EQ(expected.maxColorValue, actual.maxColorValue);
  }
}  // namespace

// Test to get metadata information
TEST(info_test, GetMetadataInfo) {
  // Define testcases
  struct TestCase {
      std::filesystem::path inputFile;
      std::string info;
  };

  std::vector<TestCase> const testCases = {
    {.inputFile = "input/deer-small.ppm",   .info = "P6 267 200 255"},
    {.inputFile = "input/deer-large.ppm", .info = "P6 8000 6000 255"},
    {.inputFile = "input/lake-large.ppm", .info = "P6 4000 3000 255"},
    {.inputFile = "input/lake-small.ppm", .info = "P6  133  100 255"},
    {  .inputFile = "input/sabatini.ppm", .info = "P6 1980 1320 255"}
  };

  for (auto const & testCase : testCases) {
    std::ifstream inputFile = openInputFile(testCase.inputFile);
    Metadata const metadata = obtainMetadata(inputFile);
    std::istringstream stream(testCase.info);
    Metadata expectedMetadata;
    stream >> expectedMetadata.format >> expectedMetadata.width >> expectedMetadata.height >>
        expectedMetadata.maxColorValue;
    compareMetadata(expectedMetadata, metadata);
  }
}