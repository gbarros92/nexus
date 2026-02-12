#pragma once
#include "math/types.hpp"

namespace math {
double dot(const Vec2& a, const Vec2& b);
Mat2   mul(const Mat2& A, const Mat2& B);
Vec2   mul(const Mat2& A, const Vec2& x);
} // namespace math
