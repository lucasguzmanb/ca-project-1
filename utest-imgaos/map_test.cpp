#include "common/binaryio.hpp"
#include "imgaos/compress.hpp"
#include "imgaos/imageaos.hpp"
#include <vector>
#include <gtest/gtest.h>


TEST(GetColorsTest, RepeatedColors) {
  // Input vector with repeated colors
  std::vector<Pixel<uint8_t>> const pixels = {
    {.r=255, .g=0, .b=0},   // Red
    {.r=0, .g=255, .b=0},   // Green
    {.r=255, .g=0, .b=0},   // Red (duplicate)
    {.r=0, .g=0, .b=255},   // Blue
    {.r=0, .g=255, .b=0},   // Green (duplicate)
    {.r=255, .g=255, .b=0}, // Yellow
    {.r=0, .g=0, .b=255},   // Blue (duplicate)
    {.r=255, .g=0, .b=0}    // Red (duplicate)
  };

  // Call the function
  auto result = getColors(pixels);

  // Expect the map size to be 4 (unique colors: Red, Green, Blue, Yellow)
  EXPECT_EQ(result.size(), 4);

  // Check that each unique pixel is correctly assigned an index
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  Pixel<uint8_t> color = {.r=255, .g=0, .b=0};
  EXPECT_EQ(result[color], 0);   // Red

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  color = {.r=0, .g=255, .b=0};
  EXPECT_EQ(result[color], 1);   // Green

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  color = {.r=0, .g=0, .b=255};
  EXPECT_EQ(result[color], 2); //Blue

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  color = {.r=255, .g=255, .b=0};
  EXPECT_EQ(result[color], 3);    // Yellow

  // Ensure no duplicates are present in the map
  for (const auto& pixel : pixels) {
    EXPECT_NE(result.find(pixel), result.end());
  }
}