#include <stdint.h>
#include "arm_neon.h"
#include "Panel/SIMD/SIMD.h"

namespace rgb_matrix::SIMD {
    template <typename T, typename R> SIMD<T, R>::SIMD(SIMD_SINGLE<T> data) {
        data_ = data;
    }

    template <typename T, typename R> SIMD<T, R> SIMD<T, R>::operator*(SIMD_SINGLE<T> const& arg) {
        // TODO:
        SIMD_SINGLE<T> r = { 0, 0, 0, 0 };
        SIMD<T, R> a(r);
        return a;
    }

    template <typename T, typename R> SIMD<T, R> SIMD<T, R>::operator*(SIMD<T, R> const& arg) {
        return *this * arg.data_;
    }

    template <typename T, typename R> SIMD<T, R> SIMD<T, R>::operator/(SIMD_SINGLE<T> const& arg) {
        // TODO:
        SIMD_SINGLE<T> r = { 0, 0, 0, 0 };
        SIMD<T, R> a(r);
        return a;
    }

    template <typename T, typename R> SIMD<T, R> SIMD<T, R>::operator/(SIMD<T, R> const& arg) {
        return *this / arg.data_;
    }

    template <typename T, typename R> SIMD_SINGLE<T> SIMD<T, R>::get() {
        return data_;
    }

    template <> SIMD_SINGLE<float> SIMD<uint32_t, float>::round() {
        // TODO:
        SIMD_SINGLE<float> r = { 0, 0, 0, 0 };
        return r;
    }

    template <> SIMD_SINGLE<uint32_t> SIMD<float, uint32_t>::round() {
        // TODO:
        SIMD_SINGLE<uint32_t> r = { 0, 0, 0, 0 };
        return r;
    }

    template class SIMD<float, uint32_t>;
    template class SIMD<uint32_t, float>;
}