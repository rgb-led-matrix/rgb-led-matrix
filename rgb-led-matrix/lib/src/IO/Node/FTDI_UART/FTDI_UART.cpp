#include <chrono>
#include <stdio.h>
#include <ftd2xx.h>
#include <Exception/Null_Pointer.h>
#include <Exception/Illegal.h>
#include <IO/Node/FTDI_UART/FTDI_UART.h>

namespace rgb_matrix::FTDI {
    // Do not use this!
    FTDI_UART::FTDI_UART() {
        throw Illegal("FTDI_UART");
    }

    FTDI_UART::FTDI_UART(const char *serial_number, uint8_t chan_num) {
        FT_HANDLE handle;
        char str[100];

        if (serial_number == nullptr)
            throw Null_Pointer("Serial Number");

        snprintf(str, sizeof(str), "%s%u", serial_number, chan_num);
        serial_number_ = str;
        claim_ = false;

        set_baud(4000000);

        if (FT_OpenEx((PVOID) serial_number_.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &handle) == FT_OK) {
            FT_SetLatencyTimer(handle, 2);
            FT_Close(handle);
        }

        // Future: Enable Hardware Flow control
    }

    void FTDI_UART::write(uint8_t *buf, uint32_t len) {
        FT_STATUS status;
        FT_HANDLE handle;
        DWORD written;

        if (buf == nullptr)
            throw Null_Pointer("Buffer");

        status = FT_OpenEx((PVOID) serial_number_.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &handle);

        if (status == FT_OK) {
            while (true) {
                status = FT_Write(handle, buf, len, &written);

                if (status == FT_OK) {
                    buf += written;
                    len -= written;

                    if (len > 0)
                        continue;
                }

                break;
            }

            FT_Close(handle);
        }
    }

    void FTDI_UART::read(uint8_t *buf, uint32_t *len, uint32_t timeout_us) {
        FT_STATUS status;
        FT_HANDLE handle;
        DWORD written;
        uint32_t timeout_ms = std::max((uint32_t) 1, timeout_us / 1000);

        if (buf == nullptr)
            throw Null_Pointer("Buffer");

        auto start = std::chrono::high_resolution_clock::now();
        status = FT_OpenEx((PVOID) serial_number_.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &handle);

        if (status == FT_OK) {
            status = FT_SetTimeouts(handle, timeout_ms, 10);

            while (status == FT_OK) {
                status = FT_Read(handle, buf, *len, &written);

                if (status == FT_OK) {
                    auto end = std::chrono::high_resolution_clock::now();

                    buf += written;
                    *len -= written;
                    
                    if (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < timeout_us)
                        break;

                    if (*len > 0)
                        continue;
                }

                break;
            }

            FT_Close(handle);
        }
    }

    bool FTDI_UART::claim() {
        bool result = false;

        lock_.lock();

        if (!claim_) {
            result = true;
            claim_ = true;
        }

        lock_.unlock();
        return result;
    }

    void FTDI_UART::free() {
        lock_.lock();
        claim_ = false;
        lock_.unlock();
    }

    void FTDI_UART::set_baud(uint32_t baud) {
        FT_HANDLE handle;

        if (FT_OpenEx((PVOID) serial_number_.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &handle) == FT_OK) {
            FT_SetBaudRate(handle, baud);
            FT_Close(handle);
        }
    }
}
