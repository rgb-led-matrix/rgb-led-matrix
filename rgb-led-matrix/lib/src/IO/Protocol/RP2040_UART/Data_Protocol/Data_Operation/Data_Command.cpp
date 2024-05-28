#include <IO/Protocol/RP2040_UART/Data_Protocol/Data_Operation/Data_Command.h>
#include <IO/Protocol/RP2040_UART/internal.h>
#include <Exception/String_Exception.h>
#include <IO/machine.h>
#include <Logger/Logger.h>

namespace rgb_matrix::Protocol::RP2040_UART {
    Data_Command::Data_Command(Node *node, uint8_t magic, bool checksum) {
        status_msg_ = Status::get_status(node, magic);
        magic_ = magic;
        checksum_ = checksum;
        node_ = node;
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
        uint32_t header = htonl(internal::generate_header(magic_));
        uint8_t cmd[2] = {'d', 'd'};
        uint16_t len = htons(length);

        Status::STATUS current = status_msg_->get_status();
        if (current != Status::STATUS::IDLE_0 && current != Status::STATUS::IDLE_1) {
            status = Data_Protocol::Status::ERROR;
            return;
        }

        // PREAMBLE_CMD_LEN_T_MULTIPLEX_COLUMNS
        if (checksum_) {
            checksum = internal::checksum_chunk(checksum, header, 32);
            checksum = internal::checksum_chunk(checksum, len, 16);
            checksum = internal::checksum_chunk(checksum, cmd[0], 8);
            checksum = internal::checksum_chunk(checksum, cmd[1], 8);
            checksum = internal::checksum_chunk(checksum, sizeof_t, 8);
            checksum = internal::checksum_chunk(checksum, multiplex, 8);
            checksum = internal::checksum_chunk(checksum, columns, 8);
            checksum = internal::checksum_chunk(checksum, format, 8);
        }

        node_->write(header);
        node_->write(len);
        node_->write(cmd[0]);
        node_->write(cmd[1]);
        node_->write(sizeof_t);
        node_->write(multiplex);
        node_->write(columns);
        node_->write(format);
        
        if (!wait(current, Status::STATUS::ACTIVE_0, 100)) {
            status = Data_Protocol::Status::ERROR;
            Logger::get_logger()->write(Logger::Level::WARN, "Data Command: Possible issue with Data Protocol.");
            Logger::get_logger()->write(Logger::Level::INFO, "Data Command: Expected transistion to ACTIVE 0 during header transfer.");
            return;
        }

        // PAYLOAD
        if (checksum_) {
            for (uint32_t i = 0; i < length; i++) {
                checksum = internal::checksum_chunk(checksum, buffer[i], 8);
            }
        }
        node_->write(buffer, length);

        if (!wait(Status::STATUS::ACTIVE_0, Status::STATUS::ACTIVE_1, 100)) {
            status = Data_Protocol::Status::ERROR;
            Logger::get_logger()->write(Logger::Level::WARN, "Data Command: Possible issue with Data Protocol.");
            Logger::get_logger()->write(Logger::Level::INFO, "Data Command: Expected transistion to ACTIVE 1 during payload transfer.");
            return;
        }

        // TODO:
        throw String_Exception("NOT FINISHED");

        status = Data_Protocol::Status::FINISHED;
    }
}