#ifndef SIMD_ARM_H
#define SIMD_ARM_H

#include <stdint.h>
#include "arm_neon.h"
#include "Panel/SIMD/SIMD_SINGLE.h"

namespace rgb_matrix::SIMD {
    SIMD_SINGLE<uint32_t> store(uint32x4_t arg);
    SIMD_SINGLE<float> store(float32x4_t arg);

    uint32x4_t load(SIMD_SINGLE<uint32_t> arg);
    float32x4_t load(SIMD_SINGLE<float> arg);
}

#endif
