#include "math/bessel.hpp"
#include <boost/math/special_functions/bessel.hpp>

namespace math {

double bessel_j0(double x) { return boost::math::cyl_bessel_j(0, x); }
double bessel_j1(double x) { return boost::math::cyl_bessel_j(1, x); }

} // namespace math
