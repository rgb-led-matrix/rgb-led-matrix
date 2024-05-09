#include <Panel/RGB/RGB.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    uint8_t RGB::translate_id(Data_Format_ID id) {
        uint8_t result;

        switch (id) {
            case Data_Format_ID::RGB24_ID:
                result = 0;
                break;
            case Data_Format_ID::RGB48_ID:
                result = 1;
                break;
            case Data_Format_ID::RGB_555_ID:
                result = 2;
                break;
            case Data_Format_ID::RGB_222_ID:
                result = 3;
                break;
            default:
                throw Unknown_Type("Data_Format_ID");
                break;
        }

        return result;
    }
}