#include "math/bessel.hpp"
#include "math/types.hpp"
#include <cmath>
#include <iostream>

static bool approx(double a, double b, double tol=1e-12) {
    return std::abs(a - b) <= tol;
}

int main() {
    // J0(0) = 1, J1(0) = 0
    if (!approx(math::bessel_j0(0.0), 1.0, 1e-14)) return 1;
    if (!approx(math::bessel_j1(0.0), 0.0, 1e-14)) return 2;

    math::Mat2 A;
    A(0,0) = math::bessel_j0(0.0);
    A(0,1) = math::bessel_j1(0.0);
    A(1,0) = math::bessel_j0(0.0);
    A(1,1) = math::bessel_j1(0.0);

    if (!approx(A(0,0), 1.0)) return 3;
    if (!approx(A(0,1), 0.0)) return 4;

    std::cout << "All tests passed.\n";
    return 0;
}
