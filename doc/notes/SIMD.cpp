// ARM gcc 10.3.0
//  -mcpu=cortex-a7 -O3 -marm -mfloat-abi=hard -mfpu=neon

#include <algorithm>
#include <mutex>
#include <stdint.h>
#include "arm_neon.h"

struct locations_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

extern uint8_t start_bit;
constexpr uint8_t PWM_bits = 11;
extern uint16_t rows;
extern uint16_t columns;
extern uint16_t gamma_lut[256];
extern locations_t *locations;
extern uint32_t *bit_planes;

const uint32x4_t lut[16] { 
    { 0 },          // 0
    { 0, 0, 0, 1 }, // 1
    { 0, 0, 1, 0 }, // 2
    { 0, 0, 1, 1 }, // 3
    { 0, 1, 0, 0 }, // 4
    { 0, 1, 0, 1 }, // 5
    { 0, 1, 1, 0 }, // 6
    { 0, 1, 1, 1 }, // 7
    { 1, 0, 0, 0 }, // 8
    { 1, 0, 0, 1 }, // 9
    { 1, 0, 1, 0 }, // 10
    { 1, 0, 1, 1 }, // 11
    { 1, 1, 0, 0 }, // 12
    { 1, 1, 0, 1 }, // 13
    { 1, 1, 1, 0 }, // 14
    { 1, 1, 1, 1 } // 15
};

// This should be small structure which can fit in L1 Data Cache
//  This will likely set an upper limit on the number of pixels supported
//  This should be close to the fanout limit for everything except GEN 1 panels
struct data_t {             // Build array/memory structure using this type as base
    //uint16_t data[18];
    uint16x8_t data[3];     // This wastes some memory
};

// This should be small and constant in L1 Data Cache
struct masks_t {
    uint32x4_t mask[18];    // This wastes some memory for speed
};

struct masks_first_gen_t {
    uint32x4_t mask[5];
};

// This will be passed thru custom L1 Data Cache IPC pipe
//  Note requires custom flow control system
struct result_t {
    uint32_t bitplanes[16];
};

static void something(data_t *data, masks_t *mask, result_t *r) {
    for (int i = 0; i < 16; i += 4) {
        uint32x4_t input[4];
        uint16x8_t data_temp;
        uint16x8_t mask_and = { 0xF };
        uint32x4_t result[4] = { { 0 } };

        if (i)
            data_temp = vshrq_n_u16(data->data[0], i);
        data_temp = vandq_u16(data_temp, mask_and);

        // This is a bottleneck
        //  This would potentially make a decent custom SIMD instruction
        input[0] = lut[data_temp[0]];
        input[1] = lut[data_temp[1]];
        input[2] = lut[data_temp[2]];
        input[3] = lut[data_temp[3]];

        // Up to 4 way superscaler
        //  Process 4 bits per operation in parallel
        //      If we have 4 way superscalar then 16 bits are in parallel potentially
        //      Note we can likely use multicore on the LED Matrix to process more bits in parallel.
        result[0] = vmlaq_u32(input[0], mask->mask[0], result[0]);
        result[1] = vmlaq_u32(input[1], mask->mask[1], result[1]);
        result[2] = vmlaq_u32(input[2], mask->mask[2], result[2]);
        result[3] = vmlaq_u32(input[3], mask->mask[3], result[3]);

        input[0] = lut[data_temp[4]];
        input[1] = lut[data_temp[5]];
        input[2] = lut[data_temp[6]];
        input[3] = lut[data_temp[7]];

        result[0] = vmlaq_u32(input[0], mask->mask[4], result[0]);
        result[1] = vmlaq_u32(input[1], mask->mask[5], result[1]);
        result[2] = vmlaq_u32(input[2], mask->mask[6], result[2]);
        result[3] = vmlaq_u32(input[3], mask->mask[7], result[3]);

        if (i)
            data_temp = vshrq_n_u16(data->data[1], i);
        data_temp = vandq_u16(data_temp, mask_and);

        input[0] = lut[data_temp[0]];
        input[1] = lut[data_temp[1]];
        input[2] = lut[data_temp[2]];
        input[3] = lut[data_temp[3]];

        result[0] = vmlaq_u32(input[0], mask->mask[8], result[0]);
        result[1] = vmlaq_u32(input[1], mask->mask[9], result[1]);
        result[2] = vmlaq_u32(input[2], mask->mask[10], result[2]);
        result[3] = vmlaq_u32(input[3], mask->mask[11], result[3]);

        input[0] = lut[data_temp[4]];
        input[1] = lut[data_temp[5]];
        input[2] = lut[data_temp[6]];
        input[3] = lut[data_temp[7]];

        result[0] = vmlaq_u32(input[0], mask->mask[12], result[0]);
        result[1] = vmlaq_u32(input[1], mask->mask[13], result[1]);
        result[2] = vmlaq_u32(input[2], mask->mask[14], result[2]);
        result[3] = vmlaq_u32(input[3], mask->mask[15], result[3]);

        if (i)
            data_temp = vshrq_n_u16(data->data[2], i);
        data_temp = vandq_u16(data_temp, mask_and);

        input[0] = lut[data_temp[0]];
        input[1] = lut[data_temp[1]];

        result[0] = vmlaq_u32(input[0], mask->mask[16], result[0]);
        result[1] = vmlaq_u32(input[1], mask->mask[17], result[1]);

        // Combine the superscalar output
        result[0] = vaddq_u32(result[0], result[1]);
        result[2] = vaddq_u32(result[2], result[3]);
        result[1] = vaddq_u32(result[0], result[2]);

        // Save off the result
        r->bitplanes[i + 0] = result[1][0];
        r->bitplanes[i + 1] = result[1][1];
        r->bitplanes[i + 2] = result[1][2];
        r->bitplanes[i + 3] = result[1][3];
    }
}

template <const int bit> static void something_first_gen(data_t *data, masks_first_gen_t *mask, uint32_t *r) {
    const int i = bit % 10;
    uint32x4_t input = { 0 };
    uint16x8_t data_temp { 0 };
    uint16x8_t mask_and = { 0xF };
    uint32x4_t result = { 0 };

    if (i)
        data_temp = vshrq_n_u16(data->data[0], i);
    data_temp = vandq_u16(data_temp, mask_and);

    // This is a bottleneck
    //  This would potentially make a decent custom SIMD instruction
    input[0] = lut[data_temp[0]][0];
    input[1] = lut[data_temp[1]][0];
    input[2] = lut[data_temp[2]][0];
    input[3] = lut[data_temp[3]][0];

    result = vmlaq_u32(input, mask->mask[0], result);

    input[0] = lut[data_temp[4]][0];
    input[1] = lut[data_temp[5]][0];
    input[2] = lut[data_temp[6]][0];
    input[3] = lut[data_temp[7]][0];

    result = vmlaq_u32(input, mask->mask[1], result);

    if (i)
        data_temp = vshrq_n_u16(data->data[1], i);
    data_temp = vandq_u16(data_temp, mask_and);

    input[0] = lut[data_temp[0]][0];
    input[1] = lut[data_temp[1]][0];
    input[2] = lut[data_temp[2]][0];
    input[3] = lut[data_temp[3]][0];

    result = vmlaq_u32(input, mask->mask[2], result);

    input[0] = lut[data_temp[4]][0];
    input[1] = lut[data_temp[5]][0];
    input[2] = lut[data_temp[6]][0];
    input[3] = lut[data_temp[7]][0];

    result = vmlaq_u32(input, mask->mask[3], result);

    if (i)
        data_temp = vshrq_n_u16(data->data[2], i);
    data_temp = vandq_u16(data_temp, mask_and);

    input[0] = lut[data_temp[0]][0];
    input[1] = lut[data_temp[1]][0];

    result = vmlaq_u32(input, mask->mask[4], result);

    // Save off the result
    *r = result[0] + result[1] + result[2] + result[3];
}

extern masks_t masks;
const int num_rows = 16;
const int num_colums = 32;

struct entry_t {
    entry_t() {
        owned = false;
    }

    result_t data;
    std::mutex lock;
    volatile bool owned;
};

// High priority thread reads this
// Low priority thread writes this
entry_t queue[4];

struct matrix_t {
    matrix_t() {
        vsync = false;
    }

    data_t data[num_rows / 2][num_colums];
    std::mutex lock;
    volatile bool vsync;
};

// Low priority thread writes this.
// High priority thread reads this.
matrix_t buffers[2];

// Likely this can be adapted to support multi-threading
void worker_thread(void *arg) {
    int i = 0;

    while (true) {
        while(!buffers[i].vsync) {}
        buffers[i].lock.lock();

        for (int r = 0; r < num_rows / 2; r++) {
            for (int c = 0; c < num_colums; c++) {
                for (int i = 0; i < 4; i++) {
                    while (queue[i].owned) {}
                    queue[i].lock.lock();
                    something(&buffers[i].data[r][c], &masks, &queue[i].data);
                    queue[i].owned = true;
                    queue[i].lock.unlock();
                }
            }
        }

        buffers[i].vsync = false;
        i = (i + 1) % 2;
        buffers[i].lock.unlock();
    }
}

extern masks_first_gen_t mask;

struct entry_first_gen_t {
    entry_first_gen_t() {
        owned = false;
    }

    uint32_t data;
    std::mutex lock;
    volatile bool owned;
};

// High priority thread reads this
// Low priority thread writes this
entry_first_gen_t queue_first_gen_t[4];

// Likely this can be adapted to support multi-threading
void worker_thread_first_gen(void *arg) {
    int i = 0;

    while (true) {
        while(!buffers[i].vsync) {}
        buffers[i].lock.lock();

        for (int b = 0; b < 11; b++) {
            for (int r = 0; r < num_rows / 2; r++) {
                for (int c = 0; c < num_colums; c++) {
                    for (int i = 0; i < 4; i++) {
                        while (queue_first_gen_t[i].owned) {}
                        queue_first_gen_t[i].lock.lock();
                        switch (b) {
                            case 0:
                                something_first_gen<0>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 1:
                                something_first_gen<1>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 2:
                                something_first_gen<2>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 3:
                                something_first_gen<3>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 4:
                                something_first_gen<4>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 5:
                                something_first_gen<5>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 6:
                                something_first_gen<6>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 7:
                                something_first_gen<7>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 8:
                                something_first_gen<8>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 9:
                                something_first_gen<9>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            case 10:
                                something_first_gen<10>(&buffers[i].data[r][c], &mask, &queue_first_gen_t[i].data);
                                break;
                            default:
                                break;
                        }
                        queue_first_gen_t[i].owned = true;
                        queue_first_gen_t[i].lock.unlock();
                    }
                }
            }
        }

        buffers[i].vsync = false;
        i = (i + 1) % 2;
        buffers[i].lock.unlock();
    }
}

void set_pixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
    locations_t location_ = locations[y * columns + x];
    uint16x4_t gamma_ = { gamma_lut[red], gamma_lut[green], gamma_lut[blue], 0 };
    uint32x4_t red_ = { location_.red };
    uint32x4_t green_ = { location_.green };
    uint32x4_t blue_ = { location_.blue };

    for (uint16_t i = start_bit; (i + 4) < PWM_bits; i += 4) {
        uint16x4_t shift_ = { i };
        uint16x4_t mask_ = { 0xF };
        gamma_ = gamma_ >> shift_;
        gamma_ = gamma_ & mask_;

        uint32x4_t red = lut[gamma_[0]] << red_;
        uint32x4_t green = lut[gamma_[1]] << green_;
        uint32x4_t blue = lut[gamma_[2]] << blue_;
        uint32x4_t pixel = red | green | blue;

        for (uint8_t j = 0; j < 4; j++) {
            bit_planes[(y * columns * PWM_bits) + ((i + j) * columns) + x] = pixel[j];
        }
    }

    uint16_t i = std::max(0, PWM_bits - ((PWM_bits / 4) * 4) - start_bit);
    if (i > 0){
        uint16x4_t shift_ = { (PWM_bits / 4) * 4 };
        uint16x4_t mask_ = { 0xF };
        gamma_ = gamma_ >> shift_;
        gamma_ = gamma_ & mask_;

        uint32x4_t red = lut[gamma_[0]] << red_;
        uint32x4_t green = lut[gamma_[1]] << green_;
        uint32x4_t blue = lut[gamma_[2]] << blue_;
        uint32x4_t pixel = red | green | blue;

        for (uint8_t j = 0; j < i; j++) {
            bit_planes[(y * columns * PWM_bits) + ((((PWM_bits / 4) * 4) + j) * columns) + x] = pixel[j];
        }
    }
}


// Type your code here, or load an example.
int square(int num) {
    static int x = 0;
    return ++x + num;
}