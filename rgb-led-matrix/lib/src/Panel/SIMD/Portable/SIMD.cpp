#include <stdint.h>
#include <cmath>
#include "Panel/SIMD/SIMD.h"

namespace rgb_matrix::SIMD {
    template <typename T> SIMD_SINGLE<T> SIMD_SINGLE<T>::operator*(SIMD_SINGLE<T> const& arg) {
        SIMD_SINGLE<T> result;

        for (int i = 0; i < 4; i++)
            result.v[i] = this->v[i] * arg.v[i];

        return result;
    }

    template <typename T> SIMD_SINGLE<T> SIMD_SINGLE<T>::operator/(SIMD_SINGLE<T> const& arg) {
        SIMD_SINGLE<T> result;

        for (int i = 0; i < 4; i++)
            result.v[i] = this->v[i] / arg.v[i];

        return result;
    }

    template <> void round(SIMD_SINGLE<uint32_t> arg, SIMD_SINGLE<float> *result) {
        for (int i = 0; i < 4; i++)
            result->v[i] = arg.v[i] * 1.0;
    }

    template <> void round(SIMD_SINGLE<float> arg, SIMD_SINGLE<uint32_t> *result) {
        for (int i = 0; i < 4; i++)
            result->v[i] = (uint32_t) std::round(arg.v[i]);
    }

    template class SIMD_SINGLE<float>;
    template class SIMD_SINGLE<uint32_t>;
}