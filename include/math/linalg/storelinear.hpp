#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

namespace nexus::math {

template <class _Scalar>
class StoreLinear {
public:
    using scalar_type = _Scalar;
    using index_type  = std::size_t;

    StoreLinear() = default;

    StoreLinear(index_type _size)
              : m_size(_size),
                m_data((_size > 0)
                       ? std::make_unique<_Scalar[]>(m_size)
                       : nullptr) {}

    StoreLinear(const StoreLinear& _other)
              : m_size(_other.m_size),
                m_data((_other.size() > 0) ? std::make_unique<_Scalar[]>(_other.size()) : nullptr) {
        if (m_data) {
            std::copy(_other.m_data.get(), _other.m_data.get() + _other.size(), m_data.get());
        }
    }

    StoreLinear& operator=(const StoreLinear& _other) {
        if (this == &_other) return *this;

        StoreLinear tmp(_other);
        swap(tmp);
        return *this;
    }

    StoreLinear(StoreLinear&& _other) noexcept = default;
    StoreLinear& operator=(StoreLinear&& _other) noexcept = default;

    ~StoreLinear() = default;

    // -------------------------
    // Basic properties
    // -------------------------
    index_type rows() const noexcept { return m_size; }
    index_type cols() const noexcept { return m_size; }

    index_type size() const noexcept { return m_size; }

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

    _Scalar& operator[](index_type _i) { return m_data[_i]; }
    const _Scalar& operator[](index_type _i) const { return m_data[_i]; }

    _Scalar& operator()(index_type _i) { return m_data[_i]; }
    const _Scalar& operator()(index_type _i) const { return m_data[_i]; }

    // -------------------------
    // Resize / clear
    // -------------------------
    void resize(index_type _size, bool _preserve = true) {
        assert(_size >= 0);

        const index_type new_size = _size;

        if (!_preserve) {
            m_size = _size;
            m_size = _size;
            m_data = (new_size > 0) ? std::make_unique<_Scalar[]>(new_size) : nullptr;
            return;
        }

        // Preserve overlapping block [0..min_rows) x [0..min_cols)
        std::unique_ptr<_Scalar[]> new_data =
                (new_size > 0) ? std::make_unique<_Scalar[]>(new_size) : nullptr;

        const index_type min_size = std::min(m_size, _size);

        for (index_type _i = 0; _i < min_size; ++_i) {
            new_data[_i * _size + _j] = at(_i, _j);
        }

        m_size = _size;
        m_data = std::move(new_data);
    }

    void clear() noexcept {
        m_size = 0;
        m_data.reset();
    }

    void swap(StoreLinear& _other) noexcept {
        std::swap(m_size, _other.m_size);
        m_data.swap(_other.m_data);
    }

private:
    [[noreturn]] static void throw_oob() {
        throw std::out_of_range("StoreLinear::at() index out of range");
    }

    index_type m_size = 0;
    std::unique_ptr<_Scalar[]> m_data;
};

}  // namespace nexus::math
