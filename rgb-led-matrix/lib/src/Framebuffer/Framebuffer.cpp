#include <algorithm>
#include <assert.h>
#include <math.h>
#include <Framebuffer/Framebuffer.h>
#include <Framebuffer/RGB/RGB24.h>
#include <Framebuffer/RGB/RGB48.h>
#include <Framebuffer/RGB/RGB_555.h>
#include <Framebuffer/RGB/RGB_222.h>
using std::min;
using std::max;

namespace rgb_matrix {
    // Don't use this!    
    template <typename T> Framebuffer<T>::Framebuffer() : cfg_(nullptr), buffer_(NULL) {
        assert(buffer_ != NULL);
    }

    template <typename T> Framebuffer<T>::Framebuffer(CFG *cfg) : cfg_(cfg) {
        assert(cfg != nullptr);

        build_table();

        buffer_ = new T *[cfg->get_cols()];
        for (int i = 0; i < cfg->get_cols(); i++)
            buffer_[i] = new T[cfg->get_rows()];

        brightness_ = 100;
    }

    template <typename T> Framebuffer<T>::~Framebuffer() {
        for (int i = 0; i < cfg_->get_cols(); i++)
            delete buffer_[i];
        delete buffer_;
    }

    template <typename T> void Framebuffer<T>::set_brightness(uint8_t brightness) {
        brightness_ = max(min(brightness, (uint8_t) 100), (uint8_t) 0);
    }

    template <typename T> void Framebuffer<T>::map_wavelength(uint8_t color, Color index, uint16_t value) {
        GAMMA g = cfg_->get_gamma();

        for (int j = 0; j < 100; j++) {
            switch (index) {
                case Color::Red:
                    lut[j][color].red = (uint16_t) round(pow(value / 65535.0, 1 / g.get_red()) * T::red_lim * j / 99.0);
                    break;
                case Color::Green:
                    lut[j][color].green = (uint16_t) round(pow(value / 65535.0, 1 / g.get_green()) * T::green_lim * j / 99.0);
                    break;
                case Color::Blue:
                    lut[j][color].blue = (uint16_t) round(pow(value / 65535.0, 1 / g.get_blue()) * T::blue_lim * j / 99.0);
                    break;
                default:
                    break;
            }
        }
    }

    template <typename T> cord_t Framebuffer<T>::get_size() {
        cord_t result;

        result.x = cfg_->get_cols();
        result.y = cfg_->get_rows();

        return result;
    }

    template<typename T> void Framebuffer<T>::show() {
        cfg_->get_node()->send((uint8_t *) buffer_, sizeof(T) * cfg_->get_cols() * cfg_->get_rows());
    }

    template <typename T> Node *Framebuffer<T>::get_node() {
        return cfg_->get_node();
    }

    template <typename T> void Framebuffer<T>::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
        T **ptr = (T **) buffer_;

        if (x > 0 && x < cfg_->get_cols() && y > 0 && y < cfg_->get_rows())
            MapColors(x, y, red, green, blue, &ptr[x][y]);
    }

    // Handles brightness and gamma
    template <typename T> void Framebuffer<T>::build_table() {
        for (uint32_t i = 0; i < 256; i++) {
            map_wavelength(i, Color::Red, i * 65536 / 256);
            map_wavelength(i, Color::Green, i * 65536 / 256);
            map_wavelength(i, Color::Blue, i * 65536 / 256);
        }
    }

    // Handles dot correction
    template <typename T> inline void Framebuffer<T>::MapColors(int x, int y, uint8_t r, uint8_t g, uint8_t b, T *pixel) {
        float fr, fg, fb;
        uint8_t bright =  this->brightness_;

        cfg_->get_dot().get(x, y, r, g, b, &fr, &fg, &fb);
        pixel->red = (uint16_t) round(this->lut[bright][r].red / T::red_max * fr);
        pixel->green = (uint16_t) round(this->lut[bright][g].green / T::green_max * fg);
        pixel->blue = (uint16_t) round(this->lut[bright][b].blue / T::blue_max * fb);
    }

    template class Framebuffer<RGB48>;
    template class Framebuffer<RGB24>;
    template class Framebuffer<RGB_555>;
    template class Framebuffer<RGB_222>;
}
