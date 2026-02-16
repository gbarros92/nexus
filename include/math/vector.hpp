#pragma once
#include <memory>
#include <ostream>
#include <type_traits>
#include <initializer_list>

namespace nexus::math
{

/**
 * @brief Fixed-size (runtime-dimension) dense vector of scalars.
 *
 * Stores @c dim() scalar entries in contiguous memory.
 *
 * @tparam _Scalar Scalar type (e.g., @c float, @c double).
 *
 * @note This class does not perform bounds checking in @ref operator[].
 *       Passing an out-of-range index results in undefined behavior.
 */
template <class _Scalar>
class Vector
{
public:
  // --------------------------------------------------------------------------
  // Constructors & Destructor
  // --------------------------------------------------------------------------

  /**
   * @brief Construct a vector of given dimension, initialized with zeros.
   * @param _dim Vector dimension (number of components).
   *
   * @pre @_dim > 0.
   */
  explicit Vector(unsigned int _dim);

  /**
   * @brief Construct a vector of given dimension and scalar value.
   * @param _dim Vector dimension (number of components).
   * @param _value Value assigned to all components.
   *
   * @pre @_dim > 0.
   */
  explicit Vector(unsigned int _dim, const _Scalar& _value);

  /**
   * @brief Copy-construct from another vector.
   * @param _vec Vector to copy from.
   */
  Vector(const Vector& _vec);

  /**
   * @brief Copy-construct from another vector of convertible scalar.
   * @param _vec Vector to copy from.
   */
  template <class T, std::enable_if_t<std::is_convertible_v<T, _Scalar>, int> = 0>
  Vector(const Vector<T>& _vec);

  /**
   * @brief Move constructor.
   * @param _vec Vector to steal from.
   */
  Vector(Vector&& _vec) noexcept;

  /**
   * @brief Construct a vector from a brace-initializer list.
   *
   * Enables:
   * @code
   * math::Vector<double> v = {1.0, 0.0, 0.0};
   * @endcode
   *
   * @param _list List of component values.
   */
  Vector(std::initializer_list<_Scalar> _list);

  /**
   * @brief Destructor.
   */
  ~Vector() = default;


  // --------------------------------------------------------------------------
  // Assignment
  // --------------------------------------------------------------------------

  /**
   * @brief Copy assignment (deep copy).
   */
  Vector& operator=(const Vector& _vec);

  /**
   * @brief Copy assignment (deep copy convertible type).
   */
  template <class T, std::enable_if_t<std::is_convertible_v<T, _Scalar>, int> = 0>
  Vector& operator=(const Vector<T>& _vec);


  /**
   * @brief Move assignment (steals resources).
   */
  Vector& operator=(Vector&& _vec) noexcept;


  // --------------------------------------------------------------------------
  // Methods
  // --------------------------------------------------------------------------

  /**
   * @brief Return the vector dimension.
   * @return Number of components in the vector.
   */
  unsigned int dim() const;


  // --------------------------------------------------------------------------
  // Subscript operators
  // --------------------------------------------------------------------------

  /**
   * @brief Element access (mutable).
   * @param _index Zero-based component index.
   * @return Reference to the component at @_index.
   *
   * @warning No bounds checking is performed.
   */
  _Scalar& operator[](unsigned int _index);

  /**
   * @brief Element access (const).
   * @param _index Zero-based component index.
   * @return Const reference to the component at @_index.
   *
   * @warning No bounds checking is performed.
   */
  const _Scalar& operator[](unsigned int _index) const;


  // --------------------------------------------------------------------------
  // Arithmetic operators
  // --------------------------------------------------------------------------

  /**
   * @brief Scalar multiplication.
   * @param _value Scalar multiplier.
   * @return New vector equal to @c (*this) * @_value.
   */
  Vector operator*(const _Scalar& _value) const;

  /**
   * @brief Scalar division.
   * @param _value Scalar divisor.
   * @return New vector equal to @c (*this) / @_value.
   *
   * @warning Behavior is undefined if @_value is zero for floating point types
   *          (or may produce Inf/NaN depending on platform/flags).
   */
  Vector operator/(const _Scalar& _value) const;

  /**
   * @brief Scalar addition.
   * @param _value Scalar to be added to all components.
   * @return New vector equal to @c (*this) + @_value.
   */
  Vector operator+(const _Scalar& _value) const;

  /**
   * @brief Scalar subtraction.
   * @param _value Scalar to be subtracted from all components.
   * @return New vector equal to @c (*this) - @_value.
   */
  Vector operator-(const _Scalar& _value) const;

  /**
   * @brief Vector addition.
   * @param _vec Right-hand side vector.
   * @return New vector equal to @c (*this) + @_vec.
   *
   * @pre @c dim() == _vec.dim()
   */
  Vector operator+(const Vector& _vec) const;

  /**
   * @brief Vector subtraction.
   * @param _vec Right-hand side vector.
   * @return New vector equal to @c (*this) - @_vec.
   *
   * @pre @c dim() == _vec.dim()
   */
  Vector operator-(const Vector& _vec) const;
  

  // --------------------------------------------------------------------------
  // Compound assignment operators
  // --------------------------------------------------------------------------

  /**
   * @brief In-place scalar multiplication.
   * @param _value Scalar multiplier.
   * @return Reference to @c *this.
   */
  Vector& operator*=(const _Scalar& _value);

  /**
   * @brief In-place scalar division.
   * @param _value Scalar divisor.
   * @return Reference to @c *this.
   */
  Vector& operator/=(const _Scalar& _value);

  /**
   * @brief In-place scalar addition.
   * @param _value Scalar to be added to all components.
   * @return Reference to @c *this.
   */
  Vector& operator+=(const _Scalar& _value);

  /**
   * @brief In-place scalar subtraction.
   * @param _value Scalar to be subtracted from all components.
   * @return Reference to @c *this.
   */
  Vector& operator-=(const _Scalar& _value);

  /**
   * @brief In-place vector addition.
   * @param _vec Right-hand side vector.
   * @return Reference to @c *this.
   *
   * @pre @c dim() == _vec.dim()
   */
  Vector& operator+=(const Vector& _vec);

  /**
   * @brief In-place vector subtraction.
   * @param _vec Right-hand side vector.
   * @return Reference to @c *this.
   *
   * @pre @c dim() == _vec.dim()
   */
  Vector& operator-=(const Vector& _vec);

private:
  // --------------------------------------------------------------------------
  // Attributes
  // --------------------------------------------------------------------------

  /// Vector dimension (number of components).
  unsigned int m_dim;

  /// Pointer to contiguous storage of length @ref _dim.
  std::unique_ptr<_Scalar[]> m_data;


  // --------------------------------------------------------------------------
  // Helper nested class
  // --------------------------------------------------------------------------
  class CommaInitializer
  {
  public:
    CommaInitializer(Vector& _vec, unsigned int _index)
      : m_vec(_vec), m_index(_index)
    {}

    CommaInitializer& operator,(const _Scalar& value)
    {
      if (m_index >= m_vec.m_dim)
        throw std::out_of_range("Too many elements in comma initializer.");

      m_vec.m_data[m_index++] = value;
      return *this;
    }

  private:
    Vector& m_vec;
    unsigned int m_index;
  };

  template <class> friend class Vector;
public:
  CommaInitializer operator<<(const _Scalar& value);
};

template <class _Scalar>
std::ostream& operator<<(std::ostream& _os, const Vector<_Scalar>& _vec);

template <class _Scalar, class T,
          std::enable_if_t<std::is_convertible_v<T, _Scalar>, int> = 0>
Vector<_Scalar> operator+(T _value, const Vector<_Scalar>& _vec);

template <class _Scalar, class T,
          std::enable_if_t<std::is_convertible_v<T, _Scalar>, int> = 0>
Vector<_Scalar> operator-(T _value, const Vector<_Scalar>& _vec);

template <class _Scalar, class T,
          std::enable_if_t<std::is_convertible_v<T, _Scalar>, int> = 0>
Vector<_Scalar> operator*(T _value, const Vector<_Scalar>& _vec);



} // namespace math

#include "vector.tpp"
