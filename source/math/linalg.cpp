#include "math/linalg.hpp"
#include <Eigen/Dense>
#include <ostream>

namespace math {

std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "[" << v.x << ", " << v.y << "]";
}

std::ostream& operator<<(std::ostream& os, const Mat2& m) {
    return os << "[[" << m(0,0) << ", " << m(0,1) << "], "
              << "[" << m(1,0) << ", " << m(1,1) << "]]";
}

double dot(const Vec2& a, const Vec2& b) {
    Eigen::Vector2d va(a.x, a.y), vb(b.x, b.y);
    return va.dot(vb);
}

Mat2 mul(const Mat2& A, const Mat2& B) {
    Eigen::Matrix2d eA, eB;
    eA << A(0,0), A(0,1), A(1,0), A(1,1);
    eB << B(0,0), B(0,1), B(1,0), B(1,1);
    Eigen::Matrix2d eC = eA * eB;

    Mat2 C;
    C(0,0)=eC(0,0); C(0,1)=eC(0,1);
    C(1,0)=eC(1,0); C(1,1)=eC(1,1);
    return C;
}

Vec2 mul(const Mat2& A, const Vec2& x) {
    Eigen::Matrix2d eA;
    eA << A(0,0), A(0,1), A(1,0), A(1,1);
    Eigen::Vector2d ex(x.x, x.y);
    Eigen::Vector2d ey = eA * ex;
    return Vec2{ey(0), ey(1)};
}

} // namespace math
