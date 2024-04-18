#include <IO/Control/COM_Control/COM_Control.h>
#include <IO/CRC/CRC.h>
#include <Exception/Illegal.h>
#include <Exception/Unknown_Type.h>

namespace rgb_matrix {
    // Do not use this!
    COM_Control::COM_Control() {
        throw Illegal("COM_Control");
    }

    COM_Control::COM_Control(Node *node) : Control(node) {
        throw String_Exception("Not finished");
        // TODO:
    }

    void COM_Control::signal(Commands command) {
        Control_Message *msg = new Control_Message(command);
        // TODO: Send it
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
            checksum = rgb_matrix::CRC::crc32(checksum, (v >> i) & 0xFF);
        
        return checksum;
    }

    inline uint32_t COM_Control::Control_Message::compute_checksum() {
        uint32_t checksum = 0xFFFFFFFF;

        checksum = checksum_chunk(checksum, header, 32);
        checksum = checksum_chunk(checksum, cmd, 8);
        checksum = checksum_chunk(checksum, len, 16);
        checksum = checksum_chunk(checksum, status, 32);

        return ~checksum;
    }
}