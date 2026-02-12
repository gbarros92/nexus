#pragma once
#include <array>
#include <cstddef>
#include <ostream>

namespace math {

struct Vec2 {
    double x{0.0}, y{0.0};
};

struct Mat2 {
    std::array<double, 4> data{0.0, 0.0, 0.0, 0.0}; // row-major

    double& operator()(std::size_t r, std::size_t c) { return data[r * 2 + c]; }
    double  operator()(std::size_t r, std::size_t c) const { return data[r * 2 + c]; }
};

std::ostream& operator<<(std::ostream& os, const Vec2& v);
std::ostream& operator<<(std::ostream& os, const Mat2& m);

} // namespace math
