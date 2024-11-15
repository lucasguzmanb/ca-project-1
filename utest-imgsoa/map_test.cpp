#include "common/binaryio.hpp"
#include "imgsoa/compress.hpp"
#include "imgsoa/imagesoa.hpp"
#include <vector>
#include <gtest/gtest.h>

// Google Test
TEST(GetColorsTest, HandlesFiveDistinctColors) {
  // Define an ImageSOA with five distinct colors
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  ImageSOA<uint8_t> image(5);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  image.setPixel(0, {255, 0, 0});   // Red
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  image.setPixel(1, {0, 255, 0});   // Green
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  image.setPixel(2, {0, 0, 255});   // Blue
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  image.setPixel(3, {255, 255, 0}); // Yellow
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  image.setPixel(4, {0, 255, 255}); // Cyan

  /*
  ImageSOA<uint8_t> colors = {
    {.r=255, .g=0, .b=0},   // Red
    {.r=0, .g=255, .b=0},   // Green
    {.r=255, .g=0, .b=0},   // Red (duplicate)
    {.r=0, .g=0, .b=255},   // Blue
    {.r=0, .g=255, .b=0},   // Green (duplicate)
    {.r=255, .g=255, .b=0}, // Yellow
    {.r=0, .g=0, .b=255},   // Blue (duplicate)
    {.r=255, .g=0, .b=0}    // Red (duplicate)
  };*/

  //ImageSOA<uint8_t> const& image;

  // Call the function
  auto colorMap = getColors(image);

  // Check the size of the color map
  ASSERT_EQ(colorMap.size(), 5);

  // Check that all colors are in the map and mapped to unique indices
  std::map<std::tuple<uint8_t, uint8_t, uint8_t>, uint32_t> const expectedMap = {
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    {{255, 0, 0}, 0},
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    {{0, 255, 0}, 1},
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    {{0, 0, 255}, 2},
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    {{255, 255, 0}, 3},
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    {{0, 255, 255}, 4}
  };

  for (const auto& pair : expectedMap) {
    auto iter = colorMap.find(pair.first);
    ASSERT_NE(iter, colorMap.end());
    EXPECT_EQ(iter->second, pair.second);
  }
}