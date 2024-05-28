#include <IO/Protocol/RP2040_UART/Data_Protocol/Data_Operation/Data_Command.h>
#include <IO/Protocol/RP2040_UART/internal.h>
#include <Exception/String_Exception.h>
#include <IO/machine.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    Data_Command::Data_Command(uint8_t magic, bool checksum) {
        status_msg_ = new Status(node, magic);
        magic = magic;
        checksum = checksum;
    }

    Data_Command::~Data_Command() {
        delete status_msg_;
    }

    bool Data_Command::wait(Status::STATUS current, Status::STATUS expected, uint32_t timeout_us) {
        while (!status_msg_->get_status(current, expected)) {
            // TODO: Check for timeout
        }

        return true;
    }

    void Data_Command::run() {
        uint32_t checksum = 0xFFFFFFFF;
        uint32_t header = htonl(internal::generate_header(magic));
        uint8_t cmd[2] = {'d', 'd'};
        uint16_t len = htons(length);

        Status::STATUS current = status_msg_->get_status();
        if (current != Status::STATUS::IDLE_0 && current != Status::STATUS::IDLE_1) {
            status = Data_Protocol::Status::ERROR;
            return;
        }

        // PREAMBLE_CMD_LEN_T_MULTIPLEX_COLUMNS
        checksum = internal::checksum_chunk(checksum, header, 32);
        node->write(header);
        checksum = internal::checksum_chunk(checksum, len, 16);
        node->write(len);
        checksum = internal::checksum_chunk(checksum, cmd[0], 8);
        node->write(cmd[0]);
        checksum = internal::checksum_chunk(checksum, cmd[1], 8);
        node->write(cmd[1]);
        checksum = internal::checksum_chunk(checksum, sizeof_t, 8);
        node->write(sizeof_t);
        checksum = internal::checksum_chunk(checksum, multiplex, 8);
        node->write(multiplex);
        checksum = internal::checksum_chunk(checksum, columns, 8);
        node->write(columns);
        checksum = internal::checksum_chunk(checksum, format, 8);
        node->write(format);
        
        if (!wait(current, Status::STATUS::ACTIVE_0, 100)) {
            status = Data_Protocol::Status::ERROR;
            return;
        }

        // PAYLOAD
        for (uint32_t i = 0; i < length; i++)
            checksum = internal::checksum_chunk(checksum, buffer[i], 8);
        node->write(buffer, length);

        if (!wait(Status::STATUS::ACTIVE_0, Status::STATUS::ACTIVE_1, 100)) {
            status = Data_Protocol::Status::ERROR;
            return;
        }

        // TODO:
        throw String_Exception("NOT FINISHED");

        status = Data_Protocol::Status::FINISHED;
    }
}