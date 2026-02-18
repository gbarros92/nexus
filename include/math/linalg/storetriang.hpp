#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>
#include <iostream>

namespace nexus::math {

struct NoDiag {
  static std::size_t size(std::size_t _n) noexcept {
    return (_n % 2 == 0) ? (_n / 2) * (_n - 1) : ((_n - 1) / 2) * _n;
  }
};

struct StoredDiag {
  static std::size_t size(std::size_t _n) noexcept {
    return (_n % 2 == 0) ? (_n / 2) * (_n + 1) : ((_n + 1) / 2) * _n;
  }
};

template <class _Scalar,
          class _DiagPolicy>
class StoreTriang {
public:
    using scalar_type = _Scalar;
    using index_type  = std::size_t;

    StoreTriang() = default;

    StoreTriang(index_type _dim)
              : m_dim(_dim),
                m_data((_dim > 0)
                       ? std::make_unique<_Scalar[]>(_DiagPolicy::size(m_dim))
                       : nullptr) {}

    StoreTriang(const StoreTriang& _other)
              : m_dim(_other.m_dim),
                m_data((_other.size() > 0) ? std::make_unique<_Scalar[]>(_other.size()) : nullptr) {
        if (m_data) {
            std::copy(_other.m_data.get(), _other.m_data.get() + _other.size(), m_data.get());
        }
    }

    StoreTriang& operator=(const StoreTriang& _other) {
        if (this == &_other) return *this;

        StoreTriang tmp(_other);
        swap(tmp);
        return *this;
    }

    StoreTriang(StoreTriang&& _other) noexcept = default;
    StoreTriang& operator=(StoreTriang&& _other) noexcept = default;

    ~StoreTriang() = default;

    // -------------------------
    // Basic properties
    // -------------------------
    index_type rows() const noexcept { return m_dim; }
    index_type cols() const noexcept { return m_dim; }

    index_type size() const noexcept { return _DiagPolicy::size(m_dim); }

    // -------------------------
    // Raw data access
    // -------------------------
    _Scalar* data() noexcept { return m_data.get(); }
    const _Scalar* data() const noexcept { return m_data.get(); }

    // -------------------------
    // Element access
    // -------------------------
    _Scalar& at(index_type _i) {
        if (_i >= size()) throw throw_oob();
        return m_data[_i];
    }

    const _Scalar& at(index_type _i) const {
        if (_i >= size()) throw throw_oob();
        return m_data[_i];
    }

    _Scalar& at(index_type _i, index_type _j) {
        if (_i >= m_dim || _j >= m_dim) throw throw_oob();
        return m_data[index(_i, _j)];
    }

    const _Scalar& at(index_type _i, index_type _j) const {
        if (_i >= m_dim || _j >= m_dim) throw throw_oob();
        return m_data[index(_i, _j)];
    }

    _Scalar& operator[](index_type _i) { return m_data[_i]; }
    const _Scalar& operator[](index_type _i) const { return m_data[_i]; }

    _Scalar& operator()(index_type _i, index_type _j) { return m_data[index(_i, _j)]; }
    const _Scalar& operator()(index_type _i, index_type _j) const { return m_data[index(_i, _j)]; }

    // -------------------------
    // Resize / clear
    // -------------------------
    void resize(index_type _dim, bool _preserve = true) {
        assert(_dim >= 0 && _dim >= 0);

        const index_type new_size = _DiagPolicy::size(_dim);

        if (!_preserve) {
            m_dim = _dim;
            m_data = (new_size > 0) ? std::make_unique<_Scalar[]>(new_size) : nullptr;
            return;
        }

        // Preserve overlapping block [0..min_rows) x [0..min_cols)
        std::unique_ptr<_Scalar[]> new_data =
                (new_size > 0) ? std::make_unique<_Scalar[]>(new_size) : nullptr;

        const index_type min_dim = std::min(m_dim, _DiagPolicy::size(_dim));

        for (index_type _i = 0; _i < min_dim; ++_i) {
            new_data[_i] = m_data[_i];
        }

        m_dim = _dim;
        m_data = std::move(new_data);
    }

    void clear() noexcept {
        m_dim = 0;
        m_data.reset();
    }

    void swap(StoreTriang& _other) noexcept {
        std::swap(m_dim, _other.m_dim);
        m_data.swap(_other.m_data);
    }

private:
    [[noreturn]] static void throw_oob() {
        throw std::out_of_range("StoreTriang::at() index out of range");
    }

    index_type index(index_type _i, index_type _j) const noexcept {
        index_type r, c;
        if (_i > _j) { r = _i; c = _j; }
        else {r = _j; c = _i;}
        // std::cout << "(" << _i << ", " << _j << ") => (" << r << ", " << c << ") =>" << _DiagPolicy::size(r) + c << std::endl;
        return _DiagPolicy::size(r) + c;
    }

    index_type m_dim = 0;
    std::unique_ptr<_Scalar[]> m_data;
};

}  // namespace nexus::math
