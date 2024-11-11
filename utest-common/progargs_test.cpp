#include "common/progargs.hpp"

#include <filesystem>
#include <gtest/gtest.h>

TEST(progargs_test, CheckMinimumArgs) {
  // Test for minimum number of arguments
  EXPECT_EXIT(check_minimum_args(1), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid number of arguments: 0");
  EXPECT_EXIT(check_minimum_args(2), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid number of arguments: 1");
  EXPECT_EXIT(check_minimum_args(3), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid number of arguments: 2");
}

TEST(progargs_test, ParseArgs) {
  // Test for parsing arguments
  auto const [input, output, operation, extra] =
      parse_args({"inputPath", "outputPath", "operation", "1", "2"});
  EXPECT_EQ(input, "inputPath");
  EXPECT_EQ(output, "outputPath");
  EXPECT_EQ(operation, "operation");
  EXPECT_EQ(extra.size(), 2);
}

TEST(progargs_test, CheckArgs) {
  // Test for invalid operation
  Arguments testCase = {.input = "input", .output = "output", .operation = "scale", .extra = {}};
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid operation: scale");

  // Test for invalid number of extra arguments (operations with no extra arguments)
  testCase = {.input = "input", .output = "output", .operation = "info", .extra = {1}};
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid number of extra arguments for info: 1");
  testCase = {.input = "input", .output = "output", .operation = "compress", .extra = {1}};
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),

              "Error: invalid number of extra arguments for compress: 1");
  // Test for invalid number of extra arguments (operations with one extra argument)
  testCase = {.input = "input", .output = "output", .operation = "maxlevel", .extra = {}};
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid number of extra arguments for maxlevel: 0");
  testCase = {.input = "input", .output = "output", .operation = "cutfreq", .extra = {}};
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid number of extra arguments for cutfreq: 0");

  // Test for invalid number of extra arguments (operations with two extra arguments)
  testCase = {.input = "input", .output = "output", .operation = "resize", .extra = {1}};
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid number of extra arguments for resize: 1");

  // Test for invalid arguments of operation "maxlevel"
  testCase = {.input = "input", .output = "output", .operation = "maxlevel", .extra = {-5}};
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid maxlevel: -5");
  testCase = {.input = "input", .output = "output", .operation = "maxlevel", .extra = {65536}};
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid maxlevel: 65536");

  // Test for invalid arguments of operation "resize"
  testCase = {
    .input = "input", .output = "output", .operation = "resize", .extra = {-10, 10}
  };
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid resize width: -10");
  testCase = {
    .input = "input", .output = "output", .operation = "resize", .extra = {10, -10}
  };
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid resize height: -10");

  // Test for invalid arguments of operation "cutfreq"
  testCase = {.input = "input", .output = "output", .operation = "cutfreq", .extra = {-1}};
  EXPECT_EXIT(check_args(testCase), ::testing::ExitedWithCode(EXIT_FAILURE),
              "Error: invalid cutfreq: -1");
}