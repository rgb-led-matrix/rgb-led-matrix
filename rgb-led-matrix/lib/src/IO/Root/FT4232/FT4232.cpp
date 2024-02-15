#include <IO/Root/FT4232/FT4232.h>
#include <IO/Node/FTDI_UART/FTDI_UART.h>
#include <Exception/Illegal.h>

namespace rgb_matrix {
    // Do not use this!
    FT4232::FT4232() {
        throw Illegal("FT4232");
    }

    FT4232::FT4232(const char *serial_number) {
        for (int i = 0; i < (sizeof(nodes_) / sizeof(Node *)); i++)
            nodes_[i] = new FTDI_UART(serial_number, i);
    }

    FT4232::~FT4232() {
        for (int i = 0; i < (sizeof(nodes_) / sizeof(Node *)); i++)
            delete nodes_[i];
    }

    Node *FT4232::GetNode(uint32_t index) {
        if (index > sizeof(nodes_) / sizeof(Node *))
            return nullptr;

        return nodes_[index];
    }
}