#ifndef SIMD_H
#define SIMD_H

namespace rgb_matrix::SIMD {
    template <typename T> class SIMD_SINGLE {   // Single is 128-bit SIMD
        public:
            SIMD_SINGLE<T> operator*(SIMD_SINGLE<T> const& arg);
            SIMD_SINGLE<T> operator/(SIMD_SINGLE<T> const& arg);

            union {
                T v[4];
                uint8_t  b[16];
                uint16_t s[8];
                uint32_t l[4];
                uint64_t ll[2];
                //uint128_t lll;  // Compiler does not support?
            };
    };

    template<typename T, typename R> void round(SIMD_SINGLE<T> arg, SIMD_SINGLE<R> *result);
}

#endif
