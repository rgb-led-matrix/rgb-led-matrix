#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>
#include <Panel/Mapper/Pixel/Pixel.h>

namespace rgb_matrix {
    // Do not use this!    
    Pixel::Pixel() : panel_(nullptr) {
        throw Illegal("Panel Pixel Mapper");
    }

    Pixel::Pixel(Panel *panel) {
        if (panel == nullptr)
            throw Null_Pointer("Panel");

        cord_t size = panel->get_size();
        panel_ = panel;
        locations_ = new cord_t *[size.x];
        orders_ = new Color_Order *[size.x];
        for (int i = 0; i < size.x; i++) {
            locations_[i] = new cord_t[size.y];
            orders_[i] = new Color_Order[size.y];
        }
        
        map();
    }   

    Pixel::~Pixel() {
        for (int i = 0; i < panel_->get_size().x; i++) {
            delete locations_[i];
            delete orders_[i];
        }
        delete locations_;
        delete orders_;
    }

    void Pixel::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
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

        cord_t cord = locations_[x][y];
    }

    void Pixel::map() {
        for (int i = 0; i < panel_->get_size().x; i++) {
            for (int j = 0; j < panel_->get_size().y; j++) {
                locations_[i][j] = map_location(i, j);
                orders_[i][j] = map_color(i, j);
            }
        }
    }
}
