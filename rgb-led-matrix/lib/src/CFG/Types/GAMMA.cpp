#include <CFG/Types/GAMMA.h>

namespace rgb_matrix {
    GAMMA::GAMMA(float red, float green, float blue) : red_(red), green_(green), blue_(blue) {
        // Do nothing
    }

    float GAMMA::get_red() { 
        return red_;
    }

    float GAMMA::get_green() { 
        return green_;
    }

    float GAMMA::get_blue() { 
        return blue_;
    }
};