#ifndef MAXLEVEL_SOA_HPP
#define MAXLEVEL_SOA_HPP
#include <vector>

template <typename T>
struct ImageSoA {
    std::vector<T> r, g, b;
};

#endif //MAXLEVEL_SOA_HPP