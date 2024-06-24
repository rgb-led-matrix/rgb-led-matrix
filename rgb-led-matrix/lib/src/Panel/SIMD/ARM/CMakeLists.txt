add_library(SIMD_ARM INTERFACE)

target_sources(SIMD_ARM INTERFACE
    SIMD_SINGLE.cpp
    SIMD_SINGLE_uint32.cpp
    SIMD_SINGLE_float32.cpp
)
