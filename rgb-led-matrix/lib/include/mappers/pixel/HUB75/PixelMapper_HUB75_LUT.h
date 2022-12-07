#ifndef HUB75_PIXEL_MAPPER_H
#define HUB75_PIXEL_MAPPER_H

#include "mappers/pixel/PixelMapper_LUT.h"

namespace rgb_matrix {
    class PixelMapper_HUB75_LUT : public PixelMapper_LUT {
        public:
            static PixelMapper_HUB75_LUT *CreateLUT();

            void RegisterPixelMapper(PixelMapper *mapper);
            std::vector<std::string> GetAvailablePixelMappers() ;
            const PixelMapper *FindPixelMapper(const char *name, int chain, int parallel, const char *parameter = NULL);

        protected:
            PixelMapper_HUB75_LUT() {}

            static PixelMapper_HUB75_LUT *ptr_;
  };
}

#endif