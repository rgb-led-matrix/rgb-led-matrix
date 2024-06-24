#include "Panel/SIMD/ARM/SIMD_SINGLE.h"

namespace rgb_matrix::SIMD {
    template <> void round(SIMD_SINGLE<uint32_t> arg, SIMD_SINGLE<float> *result) {
        *result = store(vcvtq_f32_u32(load(arg)));
    }

    template <> void round(SIMD_SINGLE<float> arg, SIMD_SINGLE<uint32_t> *result) {
        *result = store(vcvtaq_u32_f32(load(arg)));
    }
}