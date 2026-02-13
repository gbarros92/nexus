// test_vector.cpp
//
// Minimal, framework-free tests for math::Vector<T> using <cassert>.
// Compile (example):
//   g++ -std=c++17 -O0 -g test_vector.cpp -I/path/to/include -o test_vector
//
// Assumes Vector supports:
// - ctor: Vector(unsigned int dim)
// - ctor: Vector(unsigned int dim, const T& value)
// - ctor: Vector(std::initializer_list<T>)
// - copy ctor, move ctor, copy assign, move assign
// - operator[] (unchecked)
// - +, -, *, / with scalar and vectors, and compound forms
// - scalar * vector free function (in namespace math)
// - stream output operator<< (std::ostream&, const Vector<T>&)
// - Eigen-style comma initializer: v << a, b, c;
// - converting ctor Vector<U> -> Vector<T> when convertible

#include "math/vector.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>

// -------------------------
// Small test helpers
// -------------------------

#define EXPECT_TRUE(expr)                                                     \
  do {                                                                        \
    if (!(expr)) {                                                            \
      std::cerr << "EXPECT_TRUE failed: " #expr "\n"                          \
                << "  at " << __FILE__ << ":" << __LINE__ << "\n";            \
      std::abort();                                                           \
    }                                                                         \
  } while (0)

#define EXPECT_EQ(a, b)                                                       \
  do {                                                                        \
    const auto _a = (a);                                                      \
    const auto _b = (b);                                                      \
    if (!(_a == _b)) {                                                        \
      std::cerr << "EXPECT_EQ failed: " #a " == " #b "\n"                     \
                << "  got: " << _a << " vs " << _b << "\n"                    \
                << "  at " << __FILE__ << ":" << __LINE__ << "\n";            \
      std::abort();                                                           \
    }                                                                         \
  } while (0)

template <class E>
void expect_throw_invalid_arg(void (*fn)())
{
  try {
    fn();
  } catch (const std::invalid_argument&) {
    return;
  } catch (...) {
    std::cerr << "Expected std::invalid_argument, got different exception.\n";
    std::abort();
  }
  std::cerr << "Expected std::invalid_argument, got no exception.\n";
  std::abort();
}

template <class Fn>
void expect_throw_out_of_range(Fn&& fn)
{
  try {
    fn();
  } catch (const std::out_of_range&) {
    return;
  } catch (...) {
    std::cerr << "Expected std::out_of_range, got different exception.\n";
    std::abort();
  }
  std::cerr << "Expected std::out_of_range, got no exception.\n";
  std::abort();
}

template <class T>
bool almost_equal(T a, T b)
{
  if constexpr (std::is_floating_point_v<T>) {
    const T eps = T(50) * std::numeric_limits<T>::epsilon();
    const T scale = std::max<T>({T(1), std::abs(a), std::abs(b)});
    return std::abs(a - b) <= eps * scale;
  } else {
    return a == b;
  }
}

template <class T>
void expect_vec_eq(const math::Vector<T>& v, std::initializer_list<T> ref)
{
  EXPECT_EQ(v.dim(), static_cast<unsigned int>(ref.size()));
  unsigned int i = 0;
  for (const auto& x : ref) {
    EXPECT_TRUE(almost_equal<T>(v[i], x));
    ++i;
  }
}

// -------------------------
// Tests
// -------------------------

template <class T>
void test_constructors_and_access()
{
  // dim ctor (zeros)
  math::Vector<T> v(3);
  EXPECT_EQ(v.dim(), 3u);
  EXPECT_TRUE(almost_equal<T>(v[0], T{}));
  EXPECT_TRUE(almost_equal<T>(v[1], T{}));
  EXPECT_TRUE(almost_equal<T>(v[2], T{}));

  // dim + value ctor
  math::Vector<T> w(4, T(7));
  EXPECT_EQ(w.dim(), 4u);
  for (unsigned int i = 0; i < w.dim(); ++i) {
    EXPECT_TRUE(almost_equal<T>(w[i], T(7)));
  }

  // initializer_list ctor
  math::Vector<T> a = {T(1), T(2), T(3)};
  expect_vec_eq(a, {T(1), T(2), T(3)});

  // write/read via operator[]
  a[1] = T(9);
  EXPECT_TRUE(almost_equal<T>(a[1], T(9)));
}

template <class T>
void test_copy_and_move()
{
  math::Vector<T> a = {T(1), T(2), T(3)};

  // copy ctor deep copy
  math::Vector<T> b(a);
  expect_vec_eq(b, {T(1), T(2), T(3)});
  b[0] = T(99);
  EXPECT_TRUE(almost_equal<T>(b[0], T(99)));
  EXPECT_TRUE(almost_equal<T>(a[0], T(1))); // unchanged => deep copy

  // copy assignment deep copy
  math::Vector<T> c(3, T(0));
  c = a;
  expect_vec_eq(c, {T(1), T(2), T(3)});
  c[2] = T(77);
  EXPECT_TRUE(almost_equal<T>(a[2], T(3))); // unchanged

  // move ctor
  math::Vector<T> msrc = {T(4), T(5), T(6)};
  math::Vector<T> mdst(std::move(msrc));
  expect_vec_eq(mdst, {T(4), T(5), T(6)});
  EXPECT_EQ(msrc.dim(), 0u);

  // move assignment
  math::Vector<T> asrc = {T(7), T(8), T(9)};
  math::Vector<T> adst(3, T(0));
  adst = std::move(asrc);
  expect_vec_eq(adst, {T(7), T(8), T(9)});
  EXPECT_EQ(asrc.dim(), 0u);
}

template <class T>
void test_arithmetic_scalar()
{
  math::Vector<T> v = {T(1), T(2), T(3)};

  // v * scalar
  auto a = v * T(3);
  expect_vec_eq(a, {T(3), T(6), T(9)});

  // scalar * v (free function)
  auto b = T(3) * v;
  expect_vec_eq(b, {T(3), T(6), T(9)});

  // v / scalar
  if constexpr (std::is_floating_point_v<T>) {
    auto c = v / T(2);
    EXPECT_TRUE(almost_equal<T>(c[0], T(0.5)));
    EXPECT_TRUE(almost_equal<T>(c[1], T(1.0)));
    EXPECT_TRUE(almost_equal<T>(c[2], T(1.5)));
  } else {
    auto c = v / T(2);
    // integer division
    expect_vec_eq(c, {T(0), T(1), T(1)});
  }

  // compound *=
  math::Vector<T> d = {T(1), T(2), T(3)};
  d *= T(2);
  expect_vec_eq(d, {T(2), T(4), T(6)});

  // compound /=
  d /= T(2);
  expect_vec_eq(d, {T(1), T(2), T(3)});
}

template <class T>
void test_arithmetic_vector()
{
  math::Vector<T> a = {T(1), T(2), T(3)};
  math::Vector<T> b = {T(10), T(20), T(30)};

  auto c = a + b;
  expect_vec_eq(c, {T(11), T(22), T(33)});

  auto d = b - a;
  expect_vec_eq(d, {T(9), T(18), T(27)});

  // compound
  math::Vector<T> e = a;
  e += b;
  expect_vec_eq(e, {T(11), T(22), T(33)});

  e -= a;
  expect_vec_eq(e, {T(10), T(20), T(30)});
}

template <class T>
void test_dimension_mismatch_throws()
{
  math::Vector<T> a(3, T(1));
  math::Vector<T> b(4, T(1));

  expect_throw_invalid_arg<std::invalid_argument>(+[]() {
    math::Vector<T> x(0); // dim==0 should throw
  });

  // addition mismatch
  expect_throw_invalid_arg<std::invalid_argument>(+[]() {
    math::Vector<T> a(3, T(1));
    math::Vector<T> b(4, T(1));
    (void)(a + b);
  });

  // subtraction mismatch
  expect_throw_invalid_arg<std::invalid_argument>(+[]() {
    math::Vector<T> a(3, T(1));
    math::Vector<T> b(4, T(1));
    (void)(a - b);
  });

  // compound mismatch
  expect_throw_invalid_arg<std::invalid_argument>(+[]() {
    math::Vector<T> a(3, T(1));
    math::Vector<T> b(4, T(1));
    a += b;
  });

  expect_throw_invalid_arg<std::invalid_argument>(+[]() {
    math::Vector<T> a(3, T(1));
    math::Vector<T> b(4, T(1));
    a -= b;
  });
}

template <class T>
void test_stream_output()
{
  math::Vector<T> v = {T(1), T(2), T(3)};
  std::ostringstream oss;
  oss << v;
  const std::string s = oss.str();

  // Expect a bracketed, comma-separated format: [1, 2, 3]
  EXPECT_TRUE(!s.empty());
  EXPECT_TRUE(s.front() == '[');
  EXPECT_TRUE(s.back() == ']');
  EXPECT_TRUE(s.find(',') != std::string::npos);
}

template <class T>
void test_comma_initializer()
{
  math::Vector<T> k(3);
  k << T(1), T(2), T(4);
  expect_vec_eq(k, {T(1), T(2), T(4)});

  // Too many values should throw
  expect_throw_out_of_range([&]() {
    math::Vector<T> x(2);
    x << T(1), T(2), T(3);
  });

  // Initializing empty vector should throw
  expect_throw_out_of_range([&]() {
    math::Vector<T> x(0);
    x << T(1);
  });
}

void test_cross_type_copy()
{
  math::Vector<int> vi = {1, 2, 3};
  math::Vector<double> vd(vi);   // converting ctor
  expect_vec_eq(vd, {1.0, 2.0, 3.0});

  math::Vector<float> vf(3);
  vf = vd;                       // converting assignment
  EXPECT_EQ(vf.dim(), 3u);
  EXPECT_TRUE(almost_equal<float>(vf[0], 1.0f));
  EXPECT_TRUE(almost_equal<float>(vf[1], 2.0f));
  EXPECT_TRUE(almost_equal<float>(vf[2], 3.0f));
}

// -------------------------
// Main
// -------------------------

int main()
{
  // Core tests for multiple scalar types
  test_constructors_and_access<int>();
  test_copy_and_move<int>();
  test_arithmetic_scalar<int>();
  test_arithmetic_vector<int>();
  test_dimension_mismatch_throws<int>();
  test_stream_output<int>();
  test_comma_initializer<int>();

  test_constructors_and_access<float>();
  test_copy_and_move<float>();
  test_arithmetic_scalar<float>();
  test_arithmetic_vector<float>();
  test_dimension_mismatch_throws<float>();
  test_stream_output<float>();
  test_comma_initializer<float>();

  test_constructors_and_access<double>();
  test_copy_and_move<double>();
  test_arithmetic_scalar<double>();
  test_arithmetic_vector<double>();
  test_dimension_mismatch_throws<double>();
  test_stream_output<double>();
  test_comma_initializer<double>();

  test_cross_type_copy();

  std::cout << "All Vector tests passed.\n";
  return 0;
}
