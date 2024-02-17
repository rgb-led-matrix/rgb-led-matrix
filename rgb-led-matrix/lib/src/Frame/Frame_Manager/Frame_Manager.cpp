#include <Frame/Frame_Manager/Frame_Manager.h>
#include <Exception/Null_Pointer.h>

namespace rgb_matrix {
    Frame_Manager::Frame_Manager(int framerate, bool isAsync) {
        String_Exception("Not finished");

        // TODO:
    }

    Frame_Manager::~Frame_Manager() {
        // TODO:
    }

    void Frame_Manager::push_frame(Frame *frame) {
        if (frame == nullptr)
            throw Null_Pointer("Frame");

        // TODO:
    }
}
