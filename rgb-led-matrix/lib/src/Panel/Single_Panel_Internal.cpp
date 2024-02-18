#include <algorithm>
#include <map>
#include <math.h>
#include <Panel/Single_Panel_Internal.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>
#include <Panel/RGB/RGB24.h>
#include <Panel/RGB/RGB48.h>
#include <Panel/RGB/RGB_555.h>
#include <Panel/RGB/RGB_222.h>
#include <IO/Scheduler/Scheduler.h>

namespace rgb_matrix {
    // Do not use this!    
    template <typename T> Single_Panel_Internal<T>::Single_Panel_Internal() : cfg_(nullptr), buffer_(NULL) {
        throw Illegal("Single Panel Internal");
    }

    template <typename T> Single_Panel_Internal<T>::Single_Panel_Internal(CFG *cfg) : cfg_(cfg) {
        if (cfg == nullptr)
            throw Null_Pointer("CFG");

        build_table();

        buffer_ = new T *[cfg->get_cols()];
        for (uint16_t i = 0; i < cfg->get_cols(); i++)
            buffer_[i] = new T[cfg->get_rows()];

        brightness_ = 100;
    }

    template <typename T> Single_Panel_Internal<T>::~Single_Panel_Internal() {
        for (uint16_t i = 0; i < cfg_->get_cols(); i++)
            delete buffer_[i];
        delete buffer_;
    }

    template <typename T> void Single_Panel_Internal<T>::set_brightness(uint8_t brightness) {
        lock_.lock();
        // Capture the old values
        std::map<uint16_t, uint8_t> lookup[3];
        for (uint32_t i = 0; i < 256; i++) {
            lookup[0][lut[brightness_][i].red] = i;
            lookup[1][lut[brightness_][i].green] = i;
            lookup[2][lut[brightness_][i].blue] = i;
        }

        // Make the update
        brightness_ = std::max(std::min(brightness, (uint8_t) 100), (uint8_t) 0);

        // Apply the new values to the buffer
        for (uint16_t i = 0; i < cfg_->get_cols(); i++) {
            for (uint16_t j = 0; j < cfg_->get_rows(); i++) {
                buffer_[i][j].red = lut[brightness_][lookup[0][buffer_[i][j].red]].red;
                buffer_[i][j].green = lut[brightness_][lookup[1][buffer_[i][j].green]].green;
                buffer_[i][j].blue = lut[brightness_][lookup[2][buffer_[i][j].blue]].blue;
            }
        }
        lock_.unlock();
    }

    template <typename T> void Single_Panel_Internal<T>::map_wavelength(uint8_t color, Color index, uint16_t value) {
        lock_.lock();
        GAMMA g = cfg_->get_gamma();

        // Capture the old values from the table
        std::map<uint16_t, uint8_t> lookup;
        for (uint32_t i = 0; i < 256; i++) {
            switch (index) {
                case Color::Red:
                    lookup[lut[brightness_][i].red] = i;
                    break;
                case Color::Green:
                    lookup[lut[brightness_][i].green] = i;
                    break;
                case Color::Blue:
                    lookup[lut[brightness_][i].blue] = i;
                    break;
                default:
                    throw Unknown_Type("Color");
                    break;
            }
        }

        // Make the update to the table
        for (uint16_t j = 0; j < 100; j++) {
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
                    throw Unknown_Type("Color");
                    break;
            }
        }

        // Apply new values to the buffer
        for (uint16_t i = 0; i < cfg_->get_cols(); i++) {
            for (uint16_t j = 0; j < cfg_->get_rows(); i++) {
                switch (index) {
                    case Color::Red:
                        buffer_[i][j].red = lut[brightness_][lookup[buffer_[i][j].red]].red;
                        break;
                    case Color::Green:
                        buffer_[i][j].green = lut[brightness_][lookup[buffer_[i][j].green]].green;
                        break;
                    case Color::Blue:
                        buffer_[i][j].blue = lut[brightness_][lookup[buffer_[i][j].blue]].blue;
                        break;
                    default:
                        throw Unknown_Type("Color");
                        break;
                }
            }
        }
        lock_.unlock();
    }

    template <typename T> cord_t Single_Panel_Internal<T>::get_size() {
        cord_t result;
        result.x = cfg_->get_cols();
        result.y = cfg_->get_rows();
        return result;
    }

    template<typename T> void Single_Panel_Internal<T>::show(Protocol *protocol, bool schedule) {
        if (protocol == nullptr)
            throw Null_Pointer("Protocol");
        
        if (!schedule)
            protocol->send((uint8_t *) buffer_, sizeof(T) * cfg_->get_cols() * cfg_->get_rows());
        else {
            Scheduler *scheduler = new Scheduler();
            scheduler->add_protocol(protocol);
            protocol->send((uint8_t *) buffer_, sizeof(T) * cfg_->get_cols() * cfg_->get_rows());
            scheduler->start();
            delete scheduler;
        }
    }

    template <typename T> void Single_Panel_Internal<T>::SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
        T **ptr = (T **) buffer_;

        if (x > 0 && x < cfg_->get_cols() && y > 0 && y < cfg_->get_rows())
            MapColors(x, y, red, green, blue, &ptr[x][y]);
    }

    // Handles brightness and gamma
    template <typename T> void Single_Panel_Internal<T>::build_table() {
        for (uint32_t i = 0; i < 256; i++) {
            map_wavelength(i, Color::Red, i * 65536 / 256);
            map_wavelength(i, Color::Green, i * 65536 / 256);
            map_wavelength(i, Color::Blue, i * 65536 / 256);
        }
    }

    // Handles dot correction
    template <typename T> inline void Single_Panel_Internal<T>::MapColors(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, T *pixel) {
        float fr, fg, fb;

        if (pixel == nullptr)
            throw Null_Pointer("Pixel");

        lock_.lock();
        uint8_t bright =  this->brightness_;

        cfg_->get_dot().get(x, y, r, g, b, &fr, &fg, &fb);
        pixel->red = (uint16_t) round(this->lut[bright][r].red / T::red_max * fr);
        pixel->green = (uint16_t) round(this->lut[bright][g].green / T::green_max * fg);
        pixel->blue = (uint16_t) round(this->lut[bright][b].blue / T::blue_max * fb);
        lock_.unlock();
    }

    template class Single_Panel_Internal<RGB48>;
    template class Single_Panel_Internal<RGB24>;
    template class Single_Panel_Internal<RGB_555>;
    template class Single_Panel_Internal<RGB_222>;
}
