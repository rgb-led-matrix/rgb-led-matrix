#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>
#include <Panel/Panel_Pixel_Mapper.h>

namespace rgb_matrix {
    // Do not use this!    
    Panel_Pixel_Mapper::Panel_Pixel_Mapper() :panel_(nullptr) {
        throw Illegal("Panel Pixel Mapper");
    }

    Panel_Pixel_Mapper::Panel_Pixel_Mapper(Panel *panel) {
        cord_t size = panel->get_size();

        if (panel == nullptr)
            throw Null_Pointer("Panel");

        panel_ = panel;

        locations_ = new cord_t *[size.x];
        orders_ = new Color_Order *[size.x];
        for (int i = 0; i < size.x; i++) {
            locations_[i] = new cord_t[size.y];
            orders_[i] = new Color_Order[size.y];
        }
        
        map();
    }   

    Panel_Pixel_Mapper::~Panel_Pixel_Mapper() {
        for (int i = 0; i < panel_->get_size().x; i++) {
            delete locations_[i];
            delete orders_[i];
        }
        delete locations_;
        delete orders_;
    }

    void Panel_Pixel_Mapper::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
        pixel_t pixel;

        if (x < 0 || x >= panel_->get_size().x || y < 0 || y >= panel_->get_size().y)
            return;

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

        SetPixel(locations_[x][y], pixel);
    }

    void Panel_Pixel_Mapper::SetPixel(cord_t cord, pixel_t pixel) {
        panel_->SetPixel(cord.x, cord.y, pixel.red, pixel.green, pixel.blue);
    }

    void Panel_Pixel_Mapper::show() {
        panel_->show();
    }

    void Panel_Pixel_Mapper::set_brightness(uint8_t brightness) {
        panel_->set_brightness(brightness);
    }

    void Panel_Pixel_Mapper::map_wavelength(uint8_t color, Color index, uint16_t value) {
        panel_->map_wavelength(color, index, value);
    }

    cord_t Panel_Pixel_Mapper::get_size() {
        return panel_->get_size();
    }

    void Panel_Pixel_Mapper::map() {
        for (int i = 0; i < panel_->get_size().x; i++) {
            for (int j = 0; j < panel_->get_size().y; j++) {
                locations_[i][j] = map_location(i, j);
                orders_[i][j] = map_color(i, j);
            }
        }
    }
}  // namespace rgb_matrix
