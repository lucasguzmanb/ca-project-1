#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <filesystem>
#include <fstream>
#include <string>

struct Metadata {
    std::string format;
    int width;
    int height;
    int maxColorValue;
};

Metadata obtainMetadata(std::ifstream & inputFile);

std::ifstream openInputFile(std::filesystem::path const & fileName);
std::ofstream openOutputFile(std::filesystem::path const & fileName);

template <typename T>
T readBinary(std::istream & input) {
    T value;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    input.read(reinterpret_cast<char *>(&value), sizeof(value));
    return value;
}

template <typename T>
void writeBinary(std::ostream & output, T const & value) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    output.write(reinterpret_cast<char const *>(&value), sizeof(value));
}

#endif  // BINARYIO_HPP