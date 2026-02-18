#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

namespace nexus::math {

template <class _Scalar>
class StoreDense {
public:
    using scalar_type = _Scalar;
    using index_type  = std::size_t;

    StoreDense() = default;

    StoreDense(index_type _rows, index_type _cols)
              : m_rows(_rows),
                m_cols(_cols),
                m_data((_rows > 0 && _cols > 0)
                       ? std::make_unique<_Scalar[]>(m_rows * m_cols)
                       : nullptr) {}

    StoreDense(const StoreDense& _other)
              : m_rows(_other.m_rows),
                m_cols(_other.m_cols),
                m_data((_other.size() > 0) ? std::make_unique<_Scalar[]>(_other.size()) : nullptr) {
        if (m_data) {
            std::copy(_other.m_data.get(), _other.m_data.get() + _other.size(), m_data.get());
        }
    }

    StoreDense& operator=(const StoreDense& _other) {
        if (this == &_other) return *this;

        StoreDense tmp(_other);
        swap(tmp);
        return *this;
    }

    StoreDense(StoreDense&& _other) noexcept = default;
    StoreDense& operator=(StoreDense&& _other) noexcept = default;

    ~StoreDense() = default;

    // -------------------------
    // Basic properties
    // -------------------------
    index_type rows() const noexcept { return m_rows; }
    index_type cols() const noexcept { return m_cols; }

    index_type size() const noexcept { return m_rows * m_cols; }

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
        if (_i >= m_rows || _j >= m_cols) throw throw_oob();
        return m_data[index(_i, _j)];
    }

    const _Scalar& at(index_type _i, index_type _j) const {
        if (_i >= m_rows || _j >= m_cols) throw throw_oob();
        return m_data[index(_i, _j)];
    }

    _Scalar& operator[](index_type _i) { return m_data[_i]; }
    const _Scalar& operator[](index_type _i) const { return m_data[_i]; }

    _Scalar& operator()(index_type _i, index_type _j) { return m_data[index(_i, _j)]; }
    const _Scalar& operator()(index_type _i, index_type _j) const { return m_data[index(_i, _j)]; }

    // -------------------------
    // Resize / clear
    // -------------------------
    void resize(index_type _rows, index_type _cols, bool _preserve = true) {
        assert(_rows >= 0 && _cols >= 0);

        const index_type new_size = _rows * _cols;

        if (!_preserve) {
            m_rows = _rows;
            m_cols = _cols;
            m_data = (new_size > 0) ? std::make_unique<_Scalar[]>(new_size) : nullptr;
            return;
        }

        // Preserve overlapping block [0..min_rows) x [0..min_cols)
        std::unique_ptr<_Scalar[]> new_data =
                (new_size > 0) ? std::make_unique<_Scalar[]>(new_size) : nullptr;

        const index_type min_rows = std::min(m_rows, _rows);
        const index_type min_cols = std::min(m_cols, _cols);

        for (index_type _i = 0; _i < min_rows; ++_i) {
            for (index_type _j = 0; _j < min_cols; ++_j) {
                new_data[_i * _cols + _j] = at(_i, _j);
            }
        }

        m_rows = _rows;
        m_cols = _cols;
        m_data = std::move(new_data);
    }

    void clear() noexcept {
        m_rows = 0;
        m_cols = 0;
        m_data.reset();
    }

    void swap(StoreDense& _other) noexcept {
        std::swap(m_rows, _other.m_rows);
        std::swap(m_cols, _other.m_cols);
        m_data.swap(_other.m_data);
    }

private:
    [[noreturn]] static void throw_oob() {
        throw std::out_of_range("StoreDense::at() index out of range");
    }

    index_type index(index_type _i, index_type _j) const noexcept {
        return _i * m_cols + _j;
    }

    index_type m_rows = 0;
    index_type m_cols = 0;
    std::unique_ptr<_Scalar[]> m_data;
};

}  // namespace nexus::math
