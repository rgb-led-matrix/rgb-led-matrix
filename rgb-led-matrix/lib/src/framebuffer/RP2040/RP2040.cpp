#include "port/pin-mapper/RP2040/RP2040_Pins.h"
#include "framebuffer/RP2040/RP2040.h"

namespace rgb_matrix {
    RP2040::RP2040(int rows, int columns) 
        : Framebuffer(rows, columns) {

    }

    /*void RP2040::InitGPIO() {
    const struct PinMapping &h = *hardware_mapping_;
    gpio_bits_t all_used_bits = 0;
    all_used_bits |= h.TX | h.RX | h.reset | h.bootloader | h.cs_out;
    for (int i = 0; i < sizeof(hardware_mapping_->address) / sizeof(gpio_bits_t); i++)
        all_used_bits |= h.address[i];

    // Initialize outputs, make sure that all of these are supported bits.
    const gpio_bits_t result = io->InitOutputs(all_used_bits);
    assert(result == all_used_bits);  // Impl: all bits declared in gpio.cc ?
    }*/

    /*
    // TODO: Fix this
    void Framebuffer::Serialize(const char **data, size_t *len, Canvas_ID *id) const {
    *data = reinterpret_cast<const char*>(bitplane_buffer_);
    *len = buffer_size_;
    *id = id_;
    }

    // TODO: Fix this
    bool Framebuffer::Deserialize(const char *data, size_t len, Canvas_ID id) {
    if (len != buffer_size_ || id != id_) return false;
    memcpy(bitplane_buffer_, data, len);
    return true;
    }
    */
}