#include "math/vector.hpp"
#include <iostream>

int main() {
    math::Vector<double> i = {1.0, 0.0, 0.0};
    math::Vector<double> j = {0.0, 1.0, 0.0};
    math::Vector<double> k = {0.0, 0.0, 1.0};
    auto vec = i*3.0 - 2*j + false*k;

    std::cout << "v = " << vec << std::endl;
    math::Vector<double> u(3u);
    u << -3, 2, 1;
    std::cout << "u = " << u << std::endl;
    std::cout << "v+u = " << vec+u << std::endl;
    
    return 0;
}
