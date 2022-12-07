#include "mappers/pixel/HUB75/PixelMapper_HUB75_LUT.h"

namespace rgb_matrix {
    PixelMapper_HUB75_LUT *PixelMapper_HUB75_LUT::ptr_ = nullptr;

    PixelMapper_HUB75_LUT *PixelMapper_HUB75_LUT::CreateLUT() {
        if (ptr_ == nullptr)
            ptr_ = new PixelMapper_HUB75_LUT();
        
        return ptr_;
    }
}