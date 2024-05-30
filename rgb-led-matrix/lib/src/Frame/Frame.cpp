#include <Frame/Frame.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>

namespace rgb_matrix {
    Frame::Frame() {
        // Do not use this! 
    }

    Frame::Frame(Panel *panel, Data_Protocol *protocol, Control_Protocol *control) {
        if (panel == nullptr)
            throw Null_Pointer("Single Panel");

        if (protocol == nullptr)
            throw Null_Pointer("Protocol");

        isMulti_ = false;
        isFree_ = true;
        single_ = panel;
        multi_ = nullptr;
        protocol_ = protocol;
        control_ = control;
    }

    Frame::Frame(MultiPanel *panel, Control_Protocol *control) {
        if (panel == nullptr)
            throw Null_Pointer("MultiPanel");

        isMulti_ = true;
        isFree_ = true;
        single_ = nullptr;
        multi_ = panel;
        protocol_ = nullptr;
        control_ = control;
    }

    bool Frame::isFree() {
        return isFree_;
    }

    void Frame::SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
        if (!isFree())
            throw Illegal("Free");

        if (isMulti_)
            multi_->SetPixel(x, y, red, green, blue);
        else
            single_->SetPixel(x, y, red, green, blue);
    }

    cord_t Frame::get_size() {
        if (!isFree())
            throw Illegal("Free");

        if (isMulti_)
            return multi_->get_size();
        else
            return single_->get_size();
    }
    
    void Frame::set_brightness(uint8_t brightness) {
        if (!isFree())
            throw Illegal("Free");

        if (isMulti_)
            multi_->set_brightness(brightness);
        else
            single_->set_brightness(brightness);
    }
    
    void Frame::map_wavelength(uint8_t color, Color index, uint16_t value) {
        if (!isFree())
            throw Illegal("Free");

        if (isMulti_)
            multi_->map_wavelength(color, index, value);
        else
            single_->map_wavelength(color, index, value);
    }
}
