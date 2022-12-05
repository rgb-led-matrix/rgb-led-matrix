#ifndef PIXEL_MAPPER_LUT_H
#define PIXEL_MAPPER_LUT_H

#include <vector>
#include <string>
#include "pixel-mapper.h"

namespace rgb_matrix {
    class PixelMapper_LUT {
        public:
            virtual ~PixelMapper_LUT() {}

            // This is a place to register PixelMappers globally. If you register your
            // PixelMapper before calling RGBMatrix::CreateFromFlags(), the named
            // PixelMapper is available in the --led-pixel-mapper options.
            //
            // Note, you don't _have_ to register your mapper, you can always call
            // RGBMatrix::ApplyPixelMapper() directly. Registering is for convenience and
            // commandline-flag support.
            //
            // There are a few standard mappers registered by default.
            virtual void RegisterPixelMapper(PixelMapper *mapper) = 0;

            // Get a list of the names of available pixel mappers.
            virtual std::vector<std::string> GetAvailablePixelMappers() = 0;

            // Given a name (e.g. "rotate") and a parameter (e.g. "90"), return the
            // parametrized PixelMapper with that name. Returns NULL if mapper
            // can not be found or parameter is invalid.
            // Ownership of the returned object is _NOT_ transferred to the caller.
            // Current available mappers are "U-mapper" and "Rotate". The "Rotate"
            // gets a parameter denoting the angle.
            virtual const PixelMapper *FindPixelMapper(const char *name, int chain, int parallel, const char *parameter = NULL) = 0;

        protected:
            PixelMapper_LUT() {}

            static PixelMapper_LUT *ptr_;
  };
}

#endif