#include "Panel/SIMD/ARM/SIMD_SINGLE.h"

namespace rgb_matrix::SIMD {
    float32x4_t load(SIMD_SINGLE<float> arg) {
        return vld1q_f32(arg.v);
    }

    SIMD_SINGLE<float> store(float32x4_t arg) {
        SIMD_SINGLE<float> result;
        vst1q_f32(result.v, arg);
        return result;
    }

    template <> SIMD_SINGLE<float> SIMD_SINGLE<float>::operator*(SIMD_SINGLE<float> const& arg) {
        return store(vmulq_f32(load(*this), load(arg)));
    }

    template <> SIMD_SINGLE<float> SIMD_SINGLE<float>::operator/(SIMD_SINGLE<float> const& arg) {
        return store(vdivq_f32(load(*this), load(arg)));
    }

    template class SIMD_SINGLE<float>;
}