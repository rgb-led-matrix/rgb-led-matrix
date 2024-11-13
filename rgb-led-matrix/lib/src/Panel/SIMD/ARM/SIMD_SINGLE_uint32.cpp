#include "Panel/SIMD/ARM/SIMD_SINGLE.h"

namespace rgb_matrix::SIMD {
    uint32x4_t load(SIMD_SINGLE<uint32_t> arg) {
        return vld1q_u32(arg.v);
    }

    SIMD_SINGLE<uint32_t> store(uint32x4_t arg) {
        SIMD_SINGLE<uint32_t> result;
        vst1q_u32(result.v, arg);
        return result;
    }

    template <> SIMD_SINGLE<uint32_t> SIMD_SINGLE<uint32_t>::operator*(SIMD_SINGLE<uint32_t> const& arg) {
        return store(vmulq_u32(load(*this), load(arg)));
    }

    // ARM does not support this?
    template <> SIMD_SINGLE<uint32_t> SIMD_SINGLE<uint32_t>::operator/(SIMD_SINGLE<uint32_t> const& arg) {
        SIMD_SINGLE<uint32_t> result;

        for (int i = 0; i < 4; i++)
            result.v[i] = this->v[i] / arg.v[i];

        return result;
    }

    template class SIMD_SINGLE<uint32_t>;
}