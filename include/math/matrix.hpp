#pragma once

#include <cstddef>
#include "math/matrixbase.hpp"
#include "math/storedense.hpp"

namespace nexus::math {

template <class _Scalar>
class Matrix; 

template <class _Scalar>
struct matrix_traits<Matrix<_Scalar>> {
    using scalar_type = _Scalar;
    using index_type  = std::size_t;
};

template <class _Scalar>
class Matrix : public MatrixBase<Matrix<_Scalar>> {
public:
    using scalar_type = _Scalar;
    using index_type  = std::size_t;

    Matrix() = default;
    Matrix(index_type rows, index_type cols) : m_data(rows, cols) {}

    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;
    Matrix(Matrix&&) noexcept = default;
    Matrix& operator=(Matrix&&) noexcept = default;
    ~Matrix() = default;

    scalar_type& at(index_type i, index_type j)             { return m_data.at(i, j); }
    const scalar_type& at(index_type i, index_type j) const { return m_data.at(i, j); }

private:
    // Give the CRTP base access to get()/coeff() even though they're private.
    friend class MatrixBase<Matrix<_Scalar>>;

    index_type rows_impl() const noexcept { return m_data.rows(); }
    index_type cols_impl() const noexcept { return m_data.cols(); }

    scalar_type& get(index_type i)             { return m_data[i]; }
    const scalar_type& get(index_type i) const { return m_data[i]; }

    scalar_type& coeff(index_type i, index_type j)             { return m_data(i, j); }
    const scalar_type& coeff(index_type i, index_type j) const { return m_data(i, j); }

    StoreDense<_Scalar> m_data;
};

} // namespace nexus::math
