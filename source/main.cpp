#include "math/bessel.hpp"
#include "math/types.hpp"
#include <iostream>

int main() {
    math::Mat2 A;
    A(0,0) = math::bessel_j0(0.1);
    A(0,1) = math::bessel_j1(1.0);
    A(1,0) = math::bessel_j0(2.5);
    A(1,1) = math::bessel_j1(5.0);

    std::cout << "A = " << A << "\n";
    return 0;
}
