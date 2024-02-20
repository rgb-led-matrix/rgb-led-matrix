#include <Mapper/Pixel/Pixel.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!    
    Pixel::Pixel() {
        throw Illegal("Panel Pixel Mapper");
    }

    Pixel::Pixel(Multiplex *panel) {
        if (panel == nullptr)
            throw Null_Pointer("Panel");
        
        panel_ = panel;
    }  

    cord_t Pixel::get_actual_size() {
        return panel_->get_size();
    }
}
