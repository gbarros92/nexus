#pragma once
#include <ostream>
#include <tuple>

#include "matrixtraits.hpp"

namespace nexus::math {

template <class _Derived>
class MatrixBase {
public:
    using traits      = matrix_traits<_Derived>;
    using scalar_type = typename traits::scalar_type;
    using index_type  = typename traits::index_type;

    index_type rows() const noexcept { return d().rows_impl(); }
    index_type cols() const noexcept { return d().cols_impl(); }
    std::tuple<index_type, index_type> shape() const noexcept { return {d().rows_impl(), d().cols_impl()}; }

    index_type size() const noexcept { return d().m_data.size(); }

    scalar_type& operator[](index_type i)             { return d().get(i); }
    const scalar_type& operator[](index_type i) const { return d().get(i); }

    scalar_type& operator()(index_type i, index_type j)             { return d().coeff(i, j); }
    const scalar_type& operator()(index_type i, index_type j) const { return d().coeff(i, j); }

protected:
    const _Derived& d() const noexcept { return static_cast<const _Derived&>(*this); }
    _Derived& d() noexcept             { return static_cast<_Derived&>(*this); }
};


template <class _Derived>
std::ostream& operator<<(std::ostream& _os, const MatrixBase<_Derived>& _mat)
{
    for (unsigned int i = 0; i < _mat.rows(); ++i)
    {
        _os << "| ";
        for (unsigned int j = 0; j < _mat.cols(); ++j)
        {
            _os << _mat(i, j);
            if (j + 1 < _mat.cols())
                _os << ", ";
            else
                _os << " |" << std::endl;
        }
    }
  return _os;
}

} // namespace nexus::math
