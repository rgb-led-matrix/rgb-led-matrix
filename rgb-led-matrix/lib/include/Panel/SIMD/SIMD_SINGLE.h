#ifndef SIMD_SINGLE_H
#define SIMD_SINGLE_H

namespace rgb_matrix::SIMD {
    template <typename T> class SIMD_SINGLE {   // Single is 128-bit SIMD
        public:
            SIMD_SINGLE<T> operator*(SIMD_SINGLE<T> const& arg);
            SIMD_SINGLE<T> operator/(SIMD_SINGLE<T> const& arg);

            constexpr static uint32_t size() {
                return 128 / (8 * sizeof(T));
            }

            union {
                T v[SIMD_SINGLE<T>::size()];
                uint8_t  b[16];
                uint16_t s[8];
                uint32_t l[4];
                uint64_t ll[2];
                //uint128_t lll;  // Compiler does not support?
                float f[4];     // Assume IEC 60559
                double d[2];    // Assume IEC 60559
            };
    };

    template<typename T, typename R> void round(SIMD_SINGLE<T> arg, SIMD_SINGLE<R> *result);
}

#endif
