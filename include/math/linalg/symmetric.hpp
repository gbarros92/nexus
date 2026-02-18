#pragma once

#include <cstddef>
#include "matrixbase.hpp"
#include "storetriang.hpp"

namespace nexus::math {

template <class _Scalar>
class Symmetric; 

template <class _Scalar>
struct matrix_traits<Symmetric<_Scalar>> {
    using scalar_type = _Scalar;
    using index_type  = std::size_t;
};

template <class _Scalar>
class Symmetric : public MatrixBase<Symmetric<_Scalar>> {
public:
    using scalar_type = _Scalar;
    using index_type  = std::size_t;

    Symmetric() = default;
    Symmetric(index_type _dim) : m_data(_dim) {}

    Symmetric(const Symmetric&) = default;
    Symmetric& operator=(const Symmetric&) = default;
    Symmetric(Symmetric&&) noexcept = default;
    Symmetric& operator=(Symmetric&&) noexcept = default;
    ~Symmetric() = default;

    scalar_type& at(index_type i, index_type j)             { return m_data.at(i, j); }
    const scalar_type& at(index_type i, index_type j) const { return m_data.at(i, j); }

private:
    // Give the CRTP base access to get()/coeff() even though they're private.
    friend class MatrixBase<Symmetric<_Scalar>>;

    index_type rows_impl() const noexcept { return m_data.rows(); }
    index_type cols_impl() const noexcept { return m_data.cols(); }

    scalar_type& get(index_type i)             { return m_data[i]; }
    const scalar_type& get(index_type i) const { return m_data[i]; }

    scalar_type& coeff(index_type i, index_type j)             { return m_data(i, j); }
    const scalar_type& coeff(index_type i, index_type j) const { return m_data(i, j); }

    StoreTriang<_Scalar, StoredDiag> m_data;
};

} // namespace nexus::math
