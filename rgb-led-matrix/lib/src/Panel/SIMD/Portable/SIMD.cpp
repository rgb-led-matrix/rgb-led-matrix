#include <stdint.h>
#include "Panel/SIMD/SIMD.h"

namespace rgb_matrix::SIMD {
    template <typename T, typename R> SIMD<T, R>::SIMD(SIMD_SINGLE<T> data) {
        data_ = data;
    }

    template <typename T, typename R> SIMD<T, R> SIMD<T, R>::operator*(SIMD_SINGLE<T> const& arg) {
        SIMD_SINGLE<T> reg;

        for (int i = 0; i < 4; i++)
            reg.v[i] = this->data_.v[i] * arg.data_.v[i];

        SIMD<T, R> result(reg);
        return result;
    }

    template <typename T, typename R> SIMD<T, R> SIMD<T, R>::operator*(SIMD<T, R> const& arg) {
        return *this * arg.data_;
    }

    template <typename T, typename R> SIMD<T, R> SIMD<T, R>::operator/(SIMD_SINGLE<T> const& arg) {
        SIMD_SINGLE<T> reg;

        for (int i = 0; i < 4; i++)
            reg.v[i] = this->data_.v[i] / arg.data_.v[i];

        SIMD<T, R> result(reg);
        return result;
    }

    template <typename T, typename R> SIMD<T, R> SIMD<T, R>::operator/(SIMD<T, R> const& arg) {
        return *this / arg.data_;
    }

    template <typename T, typename R> SIMD_SINGLE<T> SIMD<T, R>::get() {
        return data_;
    }

    template <> SIMD_SINGLE<float> SIMD<uint32_t, float>::round() {
        SIMD_SINGLE<float> result;

        for (int i = 0; i < 4; i++)
            result.v[i] = data_.v[i] * 1.0;

        return result;
    }

    template <> SIMD_SINGLE<uint32_t> SIMD<float, uint32_t>::round() {
        SIMD_SINGLE<uint32_t> result;

        for (int i = 0; i < 4; i++)
            result.v[i] = (uint32_t) round(data_.v[i]);

        return result;
    }

    template class SIMD<float, uint32_t>;
    template class SIMD<uint32_t, float>;
}