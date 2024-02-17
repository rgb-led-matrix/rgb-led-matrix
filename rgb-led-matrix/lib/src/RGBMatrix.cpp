#include <RGBMatrix.h>
#include <CFG/CFG.h>
#include <Exception/Unknown_Type.h>
#include <Panel/Single_Panel_Internal.h>
#include <Panel/MultiPanel_Internal.h>
#include <Panel/RGB/RGB24.h>
#include <Panel/RGB/RGB48.h>
#include <Panel/RGB/RGB_555.h>
#include <Panel/RGB/RGB_222.h>

namespace rgb_matrix {
    Single_Panel *RGBMatrix::Create_Single_Panel(CFG *cfg) {
        switch (cfg->get_data_format()) {
            case Data_Format_ID::RGB48_ID:
                return new Single_Panel_Internal<RGB48>(cfg);
            case Data_Format_ID::RGB24_ID:
                return new Single_Panel_Internal<RGB24>(cfg);
            case Data_Format_ID::RGB_222_ID:
                return new Single_Panel_Internal<RGB_222>(cfg);
            case Data_Format_ID::RGB_555_ID:
                return new Single_Panel_Internal<RGB_555>(cfg);
            default:
                throw Unknown_Type("Data_Format_ID");
                break;
        }
    }

    MultiPanel *RGBMatrix::Create_MultiPanel(int width, int height) {
        return new MultiPanel_Internal(width, height);
    }
 };
