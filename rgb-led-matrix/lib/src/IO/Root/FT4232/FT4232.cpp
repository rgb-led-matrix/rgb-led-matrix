#include <IO/Root/FT4232/FT4232.h>
#include <IO/Node/FTDI_UART/FTDI_UART.h>
#include <Exception/Illegal.h>
#include <Exception/Null_Pointer.h>

namespace rgb_matrix {
    // Do not use this!
    FT4232::FT4232() {
        throw Illegal("FT4232");
    }

    FT4232::FT4232(const char *serial_number) {
        if (serial_number == nullptr)
            throw Null_Pointer("serial number");
        serial_number_ = std::string(serial_number);
    }

    Node *FT4232::GetNode(uint32_t index) {
        if (index > 4)
            throw Illegal("index");
        return new FTDI_UART(serial_number_.c_str(), index);
    }
}
