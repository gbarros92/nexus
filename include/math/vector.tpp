#pragma once

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace nexus::math
{

// ============================================================================
// Constructors
// ============================================================================

template <class _Scalar>
Vector<_Scalar>::Vector(unsigned int _dim)
  : m_dim(_dim),
    m_data(nullptr)
{
  if (m_dim == 0)
    throw std::invalid_argument("Vector dimension must be > 0.");

  m_data = std::make_unique<_Scalar[]>(m_dim);
  std::fill(m_data.get(), m_data.get() + m_dim, _Scalar{0});
}

template <class _Scalar>
Vector<_Scalar>::Vector(unsigned int _dim, const _Scalar& value)
  : m_dim(_dim),
    m_data(nullptr)
{
  if (m_dim == 0)
    throw std::invalid_argument("Vector dimension must be > 0.");

  m_data = std::make_unique<_Scalar[]>(m_dim);
  std::fill(m_data.get(), m_data.get() + m_dim, value);
}



template <class _Scalar>
Vector<_Scalar>::Vector(const Vector& _vec)
  : m_dim(_vec.m_dim),
    m_data(nullptr)
{
  if (m_dim == 0)
    return;

  m_data = std::make_unique<_Scalar[]>(m_dim);
  std::copy(_vec.m_data.get(), _vec.m_data.get() + m_dim, m_data.get());
}


template <class _Scalar>
template <class T, std::enable_if_t<std::is_convertible_v<T, _Scalar>, int>>
Vector<_Scalar>::Vector(const Vector<T>& _vec)
  : m_dim(_vec.m_dim),
    m_data(nullptr)
{
  if (m_dim == 0)
    throw std::invalid_argument("Vector dimension must be > 0.");

  m_data = std::make_unique<_Scalar[]>(m_dim);
  for (unsigned int i = 0; i < m_dim; ++i)
    m_data[i] = static_cast<_Scalar>(_vec.m_data[i]);
}


template <class _Scalar>
Vector<_Scalar>::Vector(Vector&& _vec) noexcept
  : m_dim(_vec.m_dim),
    m_data(std::move(_vec.m_data))
{
  _vec.m_dim = 0;
}


template <class _Scalar>
Vector<_Scalar>::Vector(std::initializer_list<_Scalar> _list)
  : m_dim(static_cast<unsigned int>(_list.size())),
    m_data(nullptr)
{
  if (m_dim == 0)
    throw std::invalid_argument("Initializer list cannot be empty.");

  m_data = std::make_unique<_Scalar[]>(m_dim);
  std::copy(_list.begin(), _list.end(), m_data.get());
}


// ============================================================================
// Assignment operators
// ============================================================================

template <class _Scalar>
Vector<_Scalar>& Vector<_Scalar>::operator=(const Vector& _vec)
{
  if (this == &_vec)
    return *this;

  if (_vec.m_dim == 0)
  {
    m_dim = 0;
    m_data.reset();
    return *this;
  }

  auto new_data = std::make_unique<_Scalar[]>(_vec.m_dim);
  std::copy(_vec.m_data.get(), _vec.m_data.get() + _vec.m_dim, new_data.get());

  m_dim = _vec.m_dim;
  m_data = std::move(new_data);

  return *this;
}


template <class _Scalar>
template <class T, std::enable_if_t<std::is_convertible_v<T, _Scalar>, int>>
Vector<_Scalar>& Vector<_Scalar>::operator=(const Vector<T>& _vec)
{
  if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&_vec))
    return *this; // mostly irrelevant across types, but harmless

  auto new_data = std::make_unique<_Scalar[]>(_vec.m_dim);
  for (unsigned int i = 0; i < _vec.m_dim; ++i)
    new_data[i] = static_cast<_Scalar>(_vec.m_data[i]);

  m_dim = _vec.m_dim;
  m_data = std::move(new_data);
  return *this;
}


template <class _Scalar>
Vector<_Scalar>& Vector<_Scalar>::operator=(Vector&& _vec) noexcept
{
  if (this == &_vec)
    return *this;

  m_dim = _vec.m_dim;
  m_data = std::move(_vec.m_data);

  _vec.m_dim = 0;

  return *this;
}


// ============================================================================
// Methods
// ============================================================================

template <class _Scalar>
unsigned int Vector<_Scalar>::dim() const
{
  return m_dim;
}


// ============================================================================
// Element access
// ============================================================================

template <class _Scalar>
_Scalar& Vector<_Scalar>::operator[](unsigned int index)
{
  return m_data[index];
}


template <class _Scalar>
const _Scalar& Vector<_Scalar>::operator[](unsigned int index) const
{
  return m_data[index];
}


// ============================================================================
// Arithmetic operators (non-mutating)
// ============================================================================

template <class _Scalar>
Vector<_Scalar> Vector<_Scalar>::operator*(const _Scalar& a) const
{
  Vector result(*this);
  result *= a;
  return result;
}


template <class _Scalar>
Vector<_Scalar> Vector<_Scalar>::operator/(const _Scalar& a) const
{
  Vector result(*this);
  result /= a;
  return result;
}

template <class _Scalar>
Vector<_Scalar> Vector<_Scalar>::operator+(const _Scalar& a) const
{
  Vector result(*this);
  result += a;
  return result;
}


template <class _Scalar>
Vector<_Scalar> Vector<_Scalar>::operator-(const _Scalar& a) const
{
  Vector result(*this);
  result -= a;
  return result;
}


template <class _Scalar>
Vector<_Scalar> Vector<_Scalar>::operator+(const Vector& _vec) const
{
  if (m_dim != _vec.m_dim)
    throw std::invalid_argument("Vector dimensions must match for addition.");

  Vector result(*this);
  result += _vec;
  return result;
}


template <class _Scalar>
Vector<_Scalar> Vector<_Scalar>::operator-(const Vector& _vec) const
{
  if (m_dim != _vec.m_dim)
    throw std::invalid_argument("Vector dimensions must match for subtraction.");

  Vector result(*this);
  result -= _vec;
  return result;
}


// ============================================================================
// Compound assignment operators
// ============================================================================

template <class _Scalar>
Vector<_Scalar>& Vector<_Scalar>::operator*=(const _Scalar& a)
{
  for (unsigned int i = 0; i < m_dim; ++i)
    m_data[i] *= a;

  return *this;
}


template <class _Scalar>
Vector<_Scalar>& Vector<_Scalar>::operator/=(const _Scalar& a)
{
  for (unsigned int i = 0; i < m_dim; ++i)
    m_data[i] /= a;

  return *this;
}

template <class _Scalar>
Vector<_Scalar>& Vector<_Scalar>::operator+=(const _Scalar& a)
{
  for (unsigned int i = 0; i < m_dim; ++i)
    m_data[i] += a;

  return *this;
}


template <class _Scalar>
Vector<_Scalar>& Vector<_Scalar>::operator-=(const _Scalar& a)
{
  for (unsigned int i = 0; i < m_dim; ++i)
    m_data[i] -= a;

  return *this;
}


template <class _Scalar>
Vector<_Scalar>& Vector<_Scalar>::operator+=(const Vector& _vec)
{
  if (m_dim != _vec.m_dim)
    throw std::invalid_argument("Vector dimensions must match for addition.");

  for (unsigned int i = 0; i < m_dim; ++i)
    m_data[i] += _vec.m_data[i];

  return *this;
}


template <class _Scalar>
Vector<_Scalar>& Vector<_Scalar>::operator-=(const Vector& _vec)
{
  if (m_dim != _vec.m_dim)
    throw std::invalid_argument("Vector dimensions must match for subtraction.");

  for (unsigned int i = 0; i < m_dim; ++i)
    m_data[i] -= _vec.m_data[i];

  return *this;
}


template <class _Scalar>
typename Vector<_Scalar>::CommaInitializer
Vector<_Scalar>::operator<<(const _Scalar& _value)
{
  if (m_dim == 0)
    throw std::out_of_range("Cannot initialize empty vector.");

  m_data[0] = _value;
  return CommaInitializer(*this, 1);
}


template <class _Scalar>
std::ostream& operator<<(std::ostream& _os, const Vector<_Scalar>& _vec)
{
  _os << "[";
  for (unsigned int i = 0; i < _vec.dim(); ++i)
  {
    _os << _vec[i];
    if (i + 1 < _vec.dim())
      _os << ", ";
  }
  _os << "]";
  return _os;
}

template <class _Scalar, class T,
          std::enable_if_t<std::is_convertible_v<T, _Scalar>, int>>
Vector<_Scalar> operator+(T _value, const Vector<_Scalar>& _vec)
{
  return _vec + static_cast<_Scalar>(_value);  // reuse member operator
}

template <class _Scalar, class T,
          std::enable_if_t<std::is_convertible_v<T, _Scalar>, int>>
Vector<_Scalar> operator-(T _value, const Vector<_Scalar>& _vec)
{
  return _vec - static_cast<_Scalar>(_value);  // reuse member operator
}

template <class _Scalar, class T,
          std::enable_if_t<std::is_convertible_v<T, _Scalar>, int>>
Vector<_Scalar> operator*(T _value, const Vector<_Scalar>& _vec)
{
  return _vec * static_cast<_Scalar>(_value);  // reuse member operator
}


} // namespace math
