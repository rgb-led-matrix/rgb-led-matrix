#include <Mapper/Pixel/Pixel.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!    
    Pixel::Pixel() : panel_(nullptr) {
        throw Illegal("Panel Pixel Mapper");
    }

    Pixel::Pixel(Panel *panel) {
        if (panel == nullptr)
            throw Null_Pointer("Panel");

        cord_t size = get_size();
        panel_ = panel;
        locations_ = new cord_t *[size.x];
        orders_ = new Color_Order *[size.x];
        for (uint16_t i = 0; i < size.x; i++) {
            locations_[i] = new cord_t[size.y];
            orders_[i] = new Color_Order[size.y];
        }

        panel_->resize(get_actual_size());
        
        map();
    }   

    Pixel::~Pixel() {
        for (uint16_t i = 0; i < panel_->get_size().x; i++) {
            delete locations_[i];
            delete orders_[i];
        }
        delete locations_;
        delete orders_;
    }

    void Pixel::SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
        pixel_t pixel;

        if (x >= get_size().x || y >= get_size().y)
            throw Illegal("Location");

        switch (orders_[x][y]) {
            case Color_Order::BGR:
                pixel.red = blue;
                pixel.green = green;
                pixel.blue = red;
                break;
            case Color_Order::BRG:
                pixel.red = blue;
                pixel.green = red;
                pixel.blue = green;
                break;
            case Color_Order::GBR:
                pixel.red = green;
                pixel.green = blue;
                pixel.blue = red;
                break;
            case Color_Order::GRB:
                pixel.red = green;
                pixel.green = red;
                pixel.blue = blue;
                break;
            case Color_Order::RBG:
                pixel.red = red;
                pixel.green = blue;
                pixel.blue = green;
                break;
            case Color_Order::RGB:
                pixel.red = red;
                pixel.green = green;
                pixel.blue = blue;
                break;
            default:
                throw Unknown_Type("Color_Order");
                break;
        }

        panel_->SetPixel(locations_[x][y], pixel);
    }

    void Pixel::set_brightness(uint8_t brightness) {
        panel_->set_brightness(brightness);
    }

    void Pixel::map_wavelength(uint8_t color, Color index, uint16_t value) {
        panel_->map_wavelength(color, index, value);
    }

    void Pixel::show(Protocol *protocol, bool schedule) {
        panel_->show(protocol, schedule);
    }

    void Pixel::map() {
        for (uint16_t i = 0; i < panel_->get_size().x; i++) {
            for (uint16_t j = 0; j < panel_->get_size().y; j++) {
                locations_[i][j] = map_location(i, j);
                orders_[i][j] = map_color(i, j);
            }
        }
    }
}
