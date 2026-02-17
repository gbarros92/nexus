#pragma once
#include "math/matrixtraits.hpp"

namespace nexus::math {

template <class _Derived>
class MatrixBase {
public:
    using traits      = matrix_traits<_Derived>;
    using scalar_type = typename traits::scalar_type;
    using index_type  = typename traits::index_type;

    index_type rows() const noexcept { return d().rows_impl(); }
    index_type cols() const noexcept { return d().cols_impl(); }

    index_type size() const noexcept { return rows() * cols(); }

    scalar_type& operator[](index_type i)             { return d().get(i); }
    const scalar_type& operator[](index_type i) const { return d().get(i); }

    scalar_type& operator()(index_type i, index_type j)             { return d().coeff(i, j); }
    const scalar_type& operator()(index_type i, index_type j) const { return d().coeff(i, j); }

protected:
    const _Derived& d() const noexcept { return static_cast<const _Derived&>(*this); }
    _Derived& d() noexcept             { return static_cast<_Derived&>(*this); }
};

} // namespace nexus::math
