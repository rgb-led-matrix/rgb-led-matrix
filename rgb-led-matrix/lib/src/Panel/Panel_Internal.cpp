#include <algorithm>
#include <map>
#include <math.h>
#include <Panel/Panel_Internal.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>
#include <Panel/RGB/RGB24.h>
#include <Panel/RGB/RGB48.h>
#include <Panel/RGB/RGB_555.h>
#include <Panel/RGB/RGB_222.h>
#include <Panel/RGB/RGB.h>
#include <IO/Scheduler/Scheduler.h>
#include <Panel/SIMD/SIMD.h>

namespace rgb_matrix {
    template <typename T> Panel_Internal<T>::Panel_Internal() {
        // Do not use this! 
    }

    template <typename T> Panel_Internal<T>::Panel_Internal(CFG *cfg) {
        if (cfg == nullptr)
            throw Null_Pointer("Panel Internal: Cannot create panel without configuration.");

        brightness_ = 99;
        cfg_ = cfg;
        scan_ = cfg->get_scan();
        width_ = cfg_->get_cols() * (cfg_->get_rows() / scan_);
        height_ = scan_;
        size_.x = cfg_->get_cols();
        size_.y = cfg_->get_rows();

        build_table();

        buffer_ = new T *[width_];
        for (uint16_t i = 0; i < width_; i++)
            buffer_[i] = new T[height_];

        locations_ = new cord_t *[size_.x];
        orders_ = new Color_Order *[size_.x];
        for (uint16_t i = 0; i < size_.x; i++) {
            locations_[i] = new cord_t[size_.y];
            orders_[i] = new Color_Order[size_.y];
        }

        if (cfg_->get_mapper() == nullptr)
            if (size_.x != width_)
                throw Illegal("Mapper null");

        for (uint16_t x = 0; x < size_.x; x++) {
            for (uint16_t y = 0; y < size_.y; y++) {
                if (cfg_->get_mapper() == nullptr) {
                    orders_[x][y] = Color_Order::RGB;
                    locations_[x][y].x = x;
                    locations_[x][y].y = y;
                }
                else {
                    orders_[x][y] = cfg_->get_mapper()->map_color(x, y);
                    locations_[x][y] = cfg_->get_mapper()->map_location(x, y, size_, scan_);
                    if (locations_[x][y].x >= width_ || locations_[x][y].y >= height_)
                        throw Illegal("Location");
                }
            }
        }
    }

    template <typename T> Panel_Internal<T>::~Panel_Internal() {
        for (uint16_t i = 0; i < width_; i++)
            delete buffer_[i];
        delete buffer_;

        for (uint16_t i = 0; i < size_.x; i++) {
            delete locations_[i];
            delete orders_[i];
        }
        delete locations_;
        delete orders_;
    }

    template <typename T> void Panel_Internal<T>::set_brightness(uint8_t brightness) {
        if (brightness >= 100)
            throw Illegal("Brightness");

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
        for (uint16_t i = 0; i < width_; i++) {
            for (uint16_t j = 0; j < height_; i++) {
                buffer_[i][j].red = lut[brightness_][lookup[0][buffer_[i][j].red]].red;
                buffer_[i][j].green = lut[brightness_][lookup[1][buffer_[i][j].green]].green;
                buffer_[i][j].blue = lut[brightness_][lookup[2][buffer_[i][j].blue]].blue;
            }
        }
        lock_.unlock();
    }

    template <typename T> void Panel_Internal<T>::map_wavelength(uint8_t color, Color index, uint16_t value) {
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
                    lock_.unlock();
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
                    lock_.unlock();
                    throw Unknown_Type("Color");
                    break;
            }
        }

        // Apply new values to the buffer
        for (uint16_t i = 0; i < width_; i++) {
            for (uint16_t j = 0; j < height_; i++) {
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
                        lock_.unlock();
                        throw Unknown_Type("Color");
                        break;
                }
            }
        }
        lock_.unlock();
    }

    template <typename T> cord_t Panel_Internal<T>::get_size() {
        return size_;
    }

    template<typename T> void Panel_Internal<T>::show(Data_Protocol *protocol, Control_Protocol *control, bool schedule) {
        if (protocol == nullptr)
            throw Null_Pointer("Protocol");

        lock_.lock();
        if (!schedule)
            protocol->send((void *) buffer_, sizeof(T) * width_ * height_, sizeof(T), scan_, width_, cfg_->get_data_format());
        else {
            Scheduler *scheduler = new Scheduler();
            scheduler->add_protocol(protocol);
            protocol->send((void *) buffer_, sizeof(T) * width_ * height_, sizeof(T), scan_, width_, cfg_->get_data_format());
            scheduler->start(control);
            delete scheduler;
        }
        lock_.unlock();
    }

    template <typename T> void Panel_Internal<T>::SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
        if (x >= size_.x || y >= size_.y)
            throw Illegal("Location");
        
        cord_t location = locations_[x][y];
        switch (orders_[x][y]) {
            case Color_Order::BGR:
                MapColors(location.x, location.y, blue, green, red, &buffer_[location.x][location.y]);
                break;
            case Color_Order::BRG:
                MapColors(location.x, location.y, blue, red, green, &buffer_[location.x][location.y]);
                break;
            case Color_Order::GRB:
                MapColors(location.x, location.y, green, red, blue, &buffer_[location.x][location.y]);
                break;
            case Color_Order::GBR:
                MapColors(location.x, location.y, green, blue, red, &buffer_[location.x][location.y]);
                break;
            case Color_Order::RBG:
                MapColors(location.x, location.y, red, blue, green, &buffer_[location.x][location.y]);
                break;
            case Color_Order::RGB:
                MapColors(location.x, location.y, red, green, blue, &buffer_[location.x][location.y]);
                break;
            default:
                throw Unknown_Type("Color Order");
                break;
        }
    }

    // Handles brightness and gamma
    template <typename T> void Panel_Internal<T>::build_table() {
        for (uint32_t i = 0; i < 256; i++) {
            map_wavelength(i, Color::Red, i * 65536 / 256);
            map_wavelength(i, Color::Green, i * 65536 / 256);
            map_wavelength(i, Color::Blue, i * 65536 / 256);
        }
    }

    // Handles dot correction
    template <typename T> inline void Panel_Internal<T>::MapColors(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, T *pixel) {
        rgb_matrix::SIMD::SIMD_SINGLE<float> dot, max, val;
        rgb_matrix::SIMD::SIMD_SINGLE<uint32_t> reg;

        if (pixel == nullptr)
            throw Null_Pointer("Pixel");

        lock_.lock();
        uint8_t bright = this->brightness_;
        cfg_->get_dot().get(x, y, r, g, b, &dot.v[0], &dot.v[1], &dot.v[2]);    // We do not optimize access intentionally

        {
            reg.v[0] = T::red_max;                                              // Compiler may optimize
            reg.v[1] = T::green_max;
            reg.v[2] = T::blue_max;
            rgb_matrix::SIMD::SIMD<uint32_t, float> test(reg);
            max = test.round();
        }

        {
            reg.v[0] = this->lut[bright][r].red;
            reg.v[1] = this->lut[bright][g].green;
            reg.v[2] = this->lut[bright][b].blue;
            rgb_matrix::SIMD::SIMD<uint32_t, float> test(reg);
            val = test.round();
        }

        {
            rgb_matrix::SIMD::SIMD<float, uint32_t> test(val);
            test = (test / max) * dot;
            reg = test.round();
        }

        pixel->red = reg.v[0];                                                  // We do not optimize access intentionally
        pixel->green = reg.v[1];
        pixel->blue = reg.v[2];
        lock_.unlock();
    }

    template class Panel_Internal<RGB48>;
    template class Panel_Internal<RGB24>;
    template class Panel_Internal<RGB_555>;
    template class Panel_Internal<RGB_222>;
}
