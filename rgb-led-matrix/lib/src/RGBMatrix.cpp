#include <RGBMatrix.h>
#include <CFG/CFG.h>
#include <Exception/Unknown_Type.h>
#include <Framebuffer/Framebuffer.h>
#include <Framebuffer/RGB/RGB24.h>
#include <Framebuffer/RGB/RGB48.h>
#include <Framebuffer/RGB/RGB_555.h>
#include <Framebuffer/RGB/RGB_222.h>

namespace rgb_matrix {
    Single_Panel *RGBMatrix::CreatePanel(CFG *cfg) {
        switch (cfg->get_data_format()) {
            case Data_Format_ID::RGB48_ID:
                return new Framebuffer<RGB48>(cfg);
            case Data_Format_ID::RGB24_ID:
                return new Framebuffer<RGB24>(cfg);
            case Data_Format_ID::RGB_222_ID:
                return new Framebuffer<RGB_222>(cfg);
            case Data_Format_ID::RGB_555_ID:
                return new Framebuffer<RGB_555>(cfg);
            default:
                throw Unknown_Type("Data_Format_ID");
                break;
        }
    }
};
