#ifndef SIMD_H
#define SIMD_H

namespace rgb_matrix::SIMD {
    template <typename T> struct SIMD_SINGLE {
        T v[4];
    };

    template <typename T, typename R> class SIMD {
        public:
            SIMD(SIMD_SINGLE<T> data);

            SIMD<T, R> operator*(SIMD_SINGLE<T> const& arg);
            SIMD<T, R> operator*(SIMD<T, R> const& arg);
            SIMD<T, R> operator/(SIMD_SINGLE<T> const& arg);
            SIMD<T, R> operator/(SIMD<T, R> const& arg);
            SIMD_SINGLE<R> round();

        private:
            SIMD_SINGLE<T> data_;
    };
}

#endif
