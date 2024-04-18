#include <IO/Control/COM_Control/COM_Control.h>
#include <IO/CRC/CRC.h>
#include <IO/machine.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!
    COM_Control::COM_Control() {
        throw Illegal("COM_Control");
    }

    COM_Control::COM_Control(Node *node) : Control(node) {
        // Do nothing
    }

    void COM_Control::signal(Commands command) {
        Control_Message msg(command);

        write(msg.header, sizeof(msg.header));
        write(msg.cmd, sizeof(msg.cmd));
        write(msg.len, sizeof(msg.len));
        write(msg.id, sizeof(msg.id));
        write(msg.checksum, sizeof(msg.checksum));
        write(msg.delimiter, sizeof(msg.delimiter));
    }

    void COM_Control::write(uint32_t val, uint8_t bytes) {
        char buf[4];

        if (bytes == 0 || bytes > 4)
            throw Illegal("bytes");

        for (int i = 0; i < bytes; i++)
            buf[i] = (val >> (i * 8)) & 0xFF;
        
        node_->write(buf, bytes);
    }

    COM_Control::Control_Message::Control_Message(Commands command) {
        header = htonl(0xAAEEAAEE);
        len = 1;
        id = 0;
        delimiter = htonl(0xAEAEAEAE);

        switch (command) {
            case Commands::Trigger:
                cmd = 0;
                break;
            case Commands::Reset:
                cmd = 1;
                break;
            default:
                throw Unknown_Type("Commands");
                break;
        }

        checksum = htonl(compute_checksum());
    }

    static inline uint32_t checksum_chunk(uint32_t checksum, uint32_t v, uint8_t bits) {
        for (int i = 0; i < bits; i += 8)
            checksum = CRC::crc32(checksum, (v >> i) & 0xFF);
        
        return checksum;
    }

    inline uint32_t COM_Control::Control_Message::compute_checksum() {
        uint32_t checksum = 0xFFFFFFFF;

        checksum = checksum_chunk(checksum, header, 32);
        checksum = checksum_chunk(checksum, cmd, 8);
        checksum = checksum_chunk(checksum, len, 16);
        checksum = checksum_chunk(checksum, id, 8);

        return ~checksum;
    }
}