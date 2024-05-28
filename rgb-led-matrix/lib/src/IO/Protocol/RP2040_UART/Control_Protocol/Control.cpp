#include <IO/Protocol/RP2040_UART/Control_Protocol/Control.h>
#include <IO/Protocol/RP2040_UART/internal.h>
#include <IO/machine.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    Control::Control() {
        // Do not use this! 
    }

    Control::Control(Node *node, uint8_t magic) {
        magic_ = magic;
        node_ = node;
    }

    void Control::signal(Control_Protocol::Commands command) {
        Control_Message msg(command, magic_);

        write(msg.header, sizeof(msg.header));
        write(msg.cmd, sizeof(msg.cmd));
        write(msg.len, sizeof(msg.len));
        write(msg.id, sizeof(msg.id));
        write(msg.checksum, sizeof(msg.checksum));
        write(msg.delimiter, sizeof(msg.delimiter));
    }

    void Control::write(uint32_t val, uint8_t bytes) {
        uint8_t buf[4];

        if (bytes == 0 || bytes > 4)
            throw Illegal("bytes");

        for (int i = 0; i < bytes; i++)
            buf[i] = (val >> (i * 8)) & 0xFF;
        
        node_->write(buf, bytes);
    }

    Control::Control_Message::Control_Message(Control_Protocol::Commands command, uint8_t magic) {
        header = htonl(internal::generate_header(magic));
        len = 1;
        id = 0;
        delimiter = htonl(internal::generate_delimiter(magic));

        switch (command) {
            case Control_Protocol::Commands::Trigger:
                cmd = 0;
                break;
            case Control_Protocol::Commands::Reset:
                cmd = 1;
                break;
            case Control_Protocol::Commands::Acknowledge:
                cmd = 2;
                break;
            default:
                throw Unknown_Type("Commands");
                break;
        }

        checksum = htonl(compute_checksum());
    }

    inline uint32_t Control::Control_Message::compute_checksum() {
        uint32_t checksum = 0xFFFFFFFF;

        checksum = internal::checksum_chunk(checksum, header, 32);
        checksum = internal::checksum_chunk(checksum, cmd, 8);
        checksum = internal::checksum_chunk(checksum, len, 16);
        checksum = internal::checksum_chunk(checksum, id, 8);

        return ~checksum;
    }
}