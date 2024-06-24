#ifndef SIMD_H
#define SIMD_H

namespace rgb_matrix::SIMD {
    template <typename T> class SIMD_SINGLE {
        public:
            SIMD_SINGLE<T> operator*(SIMD_SINGLE<T> const& arg);
            SIMD_SINGLE<T> operator/(SIMD_SINGLE<T> const& arg);

            T v[4];
    };

    template<typename T, typename R> void round(SIMD_SINGLE<T> arg, SIMD_SINGLE<R> *result);
}

#endif
