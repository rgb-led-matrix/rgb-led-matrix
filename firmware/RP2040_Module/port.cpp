#include <stdio.h>
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "port.h"

static void(*main_thread)(void) = NULL;

extern "C" {
    extern void OE_delay(uint32_t bits, uint32_t oe_time_ns);
    
    //void OE_delay(uint32_t bits, uint32_t oe_time_ns) {
    // for (uint32_t x = 0; x < oe_time_ns << bits; x += 32)
    //    __asm__ __volatile__ ("nop;");
    //}
}

void ns_delay(uint32_t ns) {
    OE_delay(0, ns);
}

void us_delay(uint32_t us) {
    sleep_us(us);
}

void send_pulses(uint32_t pulses) {
    for (uint32_t i = 0; i < pulses; i++) {
        gpio_set_mask(1 << 5);
        __asm__ __volatile__ ("nop;");
        gpio_clr_mask(1 << 5);
        __asm__ __volatile__ ("nop;");
    }
}

void get_string(char *str, uint32_t len) {
    fgets(str, len, stdin);
}

void launch_thread(void(*entry)(void), uint8_t priority) {
    if (priority > 0)
        multicore_launch_core1(entry);
    else
        main_thread = entry;
}

void start_scheduler() {
    if (main_thread != NULL)
        main_thread();
}

bool readInputFlag() {
    return gpio_get(8);
}

void writeOutputFlag(bool setHigh) {
    if (setHigh)
        gpioSet(1 << 8);
    else
        gpioClear(1 << 8);
}

void writeOE(bool isActive) {
    if (isActive)
        gpioClear(1 << 5);
    else
        gpioSet(1 << 5);
}

void gpioWrite(uint8_t v) {
    gpio_clr_mask(~v & 0x1F);
    gpio_set_mask(v);
}

void gpioSet(uint32_t v) {
    gpio_set_mask(v);
}
void gpioClear(uint32_t v) {
    gpio_clr_mask(v);
}

void system_init() {
    for (int i = 0; i < 29; i++)
        gpio_init(i);
    gpio_set_mask(0x1FFFFFFF);
    gpio_clr_mask(1 << 5);
    gpio_set_dir(9, false);
}
