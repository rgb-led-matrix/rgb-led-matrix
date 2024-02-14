#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <ftd2xx.h>
#include <Exception/Null_Pointer.h>
#include <IO/Node/FTDI_UART/FTDI_UART.h>
using std::min;

namespace rgb_matrix {
    FTDI_UART::FTDI_UART(const char *serial_number, uint8_t chan_num) {
        FT_HANDLE handle;
        char str[100];

        snprintf(str, sizeof(str), "%s%u", serial_number, chan_num);
        serial_number_ = str;

        set_baud(4000000);

        if (FT_OpenEx((PVOID) serial_number_.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &handle) == FT_OK) {
            FT_SetLatencyTimer(handle, 2);
            FT_Close(handle);
        }
    }

    void FTDI_UART::write(char *buf, uint32_t len) {
        FT_STATUS status;
        FT_HANDLE handle;
        DWORD written;

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

    int FTDI_UART::read(char **buf, uint32_t len, uint32_t timeout_us) {
        FT_STATUS status;
        FT_HANDLE handle;
        DWORD written;
        uint32_t timeout_ms = std::max((uint32_t) 1, timeout_us / 1000);

        auto start = std::chrono::high_resolution_clock::now();
        status = FT_OpenEx((PVOID) serial_number_.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &handle);

        if (status == FT_OK) {
            status = FT_SetTimeouts(handle, timeout_ms, 10);

            while (status == FT_OK) {
                status = FT_Read(handle, buf, len, &written);

                if (status == FT_OK) {
                    auto end = std::chrono::high_resolution_clock::now();
                    
                    if (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < timeout_us)
                        return 1;
                    
                    buf += written;
                    len -= written;

                    if (len > 0)
                        continue;
                }

                break;
            }

            FT_Close(handle);
        }

        return 0;
    }

    void FTDI_UART::set_baud(uint32_t baud) {
        FT_HANDLE handle;

        lock_.lock();

        if (FT_OpenEx((PVOID) serial_number_.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &handle) == FT_OK) {
            FT_SetBaudRate(handle, baud);
            FT_Close(handle);
        }

        lock_.unlock();
    }


    void FTDI_UART::send(uint8_t *buf, uint32_t size) {
        lock_.lock();
        if (protocol_ == nullptr)
            throw Null_Pointer("Protocol");

        protocol_->send(buf, size, this);
        lock_.unlock();
    }
}
