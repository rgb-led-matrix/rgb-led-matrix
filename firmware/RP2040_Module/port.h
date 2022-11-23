#include "stdint.h"

void system_init();

// Should use timer or dedicated thread
void ns_delay(uint32_t ns);

// Should use timer or dedicated thread
void us_delay(uint32_t us);

// Should use hardware with DMA or dedicated thread
void send_pulses(uint32_t pulses);

// Should use hardware with DMA or dedicated thread
void get_string(char *str, uint32_t len);

void launch_thread(void(*entry)(void), uint8_t priority);

void start_scheduler();

bool readInputFlag();

void writeOutputFlag(bool setHigh);

void writeOE(bool isActive);

void gpioWrite(uint8_t v);

void gpioSet(uint32_t v);

void gpioClear(uint32_t v);
