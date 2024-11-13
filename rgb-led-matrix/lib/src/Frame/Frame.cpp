#include "Frame/Frame.h"
#include "Exception/Null_Pointer.h"

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
        single_ = panel;
        multi_ = nullptr;
        protocol_ = protocol;
        control_ = control;
        isFree_ = true;
    }

    Frame::Frame(MultiPanel *panel, Control_Protocol *control) {
        if (panel == nullptr)
            throw Null_Pointer("MultiPanel");

        isMulti_ = true;
        single_ = nullptr;
        multi_ = panel;
        protocol_ = nullptr;
        control_ = control;
        isFree_ = true;
    }

    void Frame::show(uint64_t key) {
        lock_.lock();
        if (key == 0x12345678) {
            isFree_ = false;

            if (isMulti_)
                multi_->show(control_);
            else
                single_->show(protocol_, control_);
        }
        lock_.unlock();
    }

    bool Frame::isFree() {
        return isFree_;
    }

    void Frame::SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
        lock_.lock();
        if (isMulti_)
            multi_->SetPixel(x, y, red, green, blue);
        else
            single_->SetPixel(x, y, red, green, blue);
        lock_.unlock();
    }

    cord_t Frame::get_size() {
        lock_.lock();
        if (isMulti_)
            return multi_->get_size();
        else
            return single_->get_size();
        lock_.unlock();
    }
    
    void Frame::set_brightness(uint8_t brightness) {
        lock_.lock();
        if (isMulti_)
            multi_->set_brightness(brightness);
        else
            single_->set_brightness(brightness);
        lock_.unlock();
    }
    
    void Frame::map_wavelength(uint8_t color, Color index, uint16_t value) {
        lock_.lock();
        if (isMulti_)
            multi_->map_wavelength(color, index, value);
        else
            single_->map_wavelength(color, index, value);
        lock_.unlock();
    }
}
