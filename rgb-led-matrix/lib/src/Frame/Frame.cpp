#include <Frame/Frame.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>

namespace rgb_matrix {
    // Do not use this!    
    Frame::Frame() {
        throw Illegal("Frame");
    }

    Frame::Frame(Panel *panel, Protocol *protocol) {
        if (panel == nullptr)
            throw Null_Pointer("Single Panel");

        if (protocol == nullptr)
            throw Null_Pointer("Protocol");

        isFree_ = true;
        single_ = panel;
        protocol_ = protocol;
    }


    bool Frame::isFree() {
        return isFree_;
    }

    void Frame::SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
        if (!isFree())
            throw Illegal("Free");

        single_->SetPixel(x, y, red, green, blue);
    }

    cord_t Frame::get_size() {
        if (!isFree())
            throw Illegal("Free");

        return single_->get_size();
    }
    
    void Frame::set_brightness(uint8_t brightness) {
        if (!isFree())
            throw Illegal("Free");

        single_->set_brightness(brightness);
    }
    
    void Frame::map_wavelength(uint8_t color, Color index, uint16_t value) {
        if (!isFree())
            throw Illegal("Free");

        single_->map_wavelength(color, index, value);
    }
}
