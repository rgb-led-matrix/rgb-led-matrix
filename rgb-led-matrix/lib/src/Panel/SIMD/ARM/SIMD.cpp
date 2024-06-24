#include <stdint.h>
#include "arm_neon.h"
#include "Panel/SIMD/SIMD.h"

namespace rgb_matrix::SIMD {
    static inline uint32x4_t load(SIMD_SINGLE<uint32_t> arg) {
        return vld1q_u32(arg.v);
    }

    static inline float32x4_t load(SIMD_SINGLE<float> arg) {
        return vld1q_f32(arg.v);
    }

    static inline SIMD_SINGLE<uint32_t> store(uint32x4_t arg) {
        SIMD_SINGLE<uint32_t> result;
        vst1q_u32(result.v, arg);
        return result;
    }

    static inline SIMD_SINGLE<float> store(float32x4_t arg) {
        SIMD_SINGLE<float> result;
        vst1q_f32(result.v, arg);
        return result;
    }

    template <> SIMD_SINGLE<uint32_t> SIMD_SINGLE<uint32_t>::operator*(SIMD_SINGLE<uint32_t> const& arg) {
        return store(vmulq_u32(load(*this), load(arg)));
    }

    template <> SIMD_SINGLE<float> SIMD_SINGLE<float>::operator*(SIMD_SINGLE<float> const& arg) {
        return store(vmulq_f32(load(*this), load(arg)));
    }

    // ARM does not support this?
    template <> SIMD_SINGLE<uint32_t> SIMD_SINGLE<uint32_t>::operator/(SIMD_SINGLE<uint32_t> const& arg) {
        SIMD_SINGLE<uint32_t> result;

        for (int i = 0; i < 4; i++)
            result.v[i] = this->v[i] / arg.v[i];

        return result;
    }

    template <> SIMD_SINGLE<float> SIMD_SINGLE<float>::operator/(SIMD_SINGLE<float> const& arg) {
        return store(vdivq_f32(load(*this), load(arg)));
    }

    template <> void round(SIMD_SINGLE<uint32_t> arg, SIMD_SINGLE<float> *result) {
        // TODO:
        //vcvtq_f32_u32;
    }

    template <> void round(SIMD_SINGLE<float> arg, SIMD_SINGLE<uint32_t> *result) {
        // TODO:
        //vcvtaq_u32_f32;
    }

    template class SIMD_SINGLE<float>;
    template class SIMD_SINGLE<uint32_t>;
}