#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "port.h"
#include "Multiplex/Multiplex.h"

typedef union {
    struct {
        uint32_t multiplex;
        uint32_t multiplexer;
        uint32_t grayscaler;
        uint32_t blank_time_us;
        uint32_t gclk_steps;
        uint32_t oe_time_ns;
        uint32_t oe_bits;
    } cfg;
    uint32_t v[7];
} CFG_t;

static CFG_t cfg;
static bool shutdown;
static Multiplex *multiplexer;

static void OE();
static void GCLK();
static bool getCFG(CFG_t *cfg);
static void main_thread();

int main() { 
    stdio_init_all();
    system_init();
    launch_thread(main_thread, 0);
    start_scheduler();
}

void main_thread() {
    static CFG_t c;
    
    while (getCFG(&c));
    shutdown = false;
    cfg = c;
    if (cfg.cfg.grayscaler)
        launch_thread(OE, 1);
    else
        launch_thread(GCLK, 1);
    printf("Valid String\n");
        
    while (1) {
        while (getCFG(&c));
        shutdown = true;
        while (shutdown);
        cfg = c;
        if (cfg.cfg.grayscaler)
            launch_thread(OE, 1);
        else
            launch_thread(GCLK, 1);
        printf("Valid String\n");
    }
}

void GCLK() {
    uint32_t counter = 0;
    multiplexer = Multiplex::getMultiplexer(cfg.cfg.multiplex, cfg.cfg.multiplexer);

    while (!shutdown) {
        if (readInputFlag()) {                          // Check if host is ready for VSYNC
            // TODO: Verify if a certain number of GCLK pulses are needed here
            counter = 0;
            multiplexer->SetRow(counter);               // Reset to first row
            writeOutputFlag(true);                      // Tell host we are ready for VSYNC
            while(!readInputFlag());                    // Wait for host to send VSYNC
            // TODO: Verify if a usleep delay is needed here
            writeOutputFlag(false);                     // Tell host we have resumed multiplexing
        }
        send_pulses(cfg.cfg.gclk_steps);                // Clock PWM counters for Scrambled PWM or Traditional PWM
        us_delay(cfg.cfg.blank_time_us);                // Enter dead time (Display disabled). Let ghosting logic work. Allow SRAM for next row to load.
        multiplexer->SetRow(counter);                   // Change the row
        counter = (counter + 1) % cfg.cfg.multiplex;
    }
    
    delete multiplexer;
    shutdown = false;
}

void OE() {
    uint32_t counter = 0;
    uint32_t bits = 0;
    multiplexer = Multiplex::getMultiplexer(cfg.cfg.multiplex, cfg.cfg.multiplexer);

    while (!shutdown) {
        writeOutputFlag(true);                          // Tell host we are ready for next bitplane
        while(readInputFlag());                         // Wait for host to be ready
        while(!readInputFlag());                        // Wait for host to latch data
        writeOutputFlag(false);                         // Tell host we are still working on current bitplane
        writeOE(true);                                  // Enable display
        ns_delay(cfg.cfg.oe_time_ns << bits);           // Delay
        writeOE(false);                                 // Disable display
        if (++bits >= cfg.cfg.oe_bits) {                // Check if we need to change the row
            bits = 0;
            us_delay(cfg.cfg.blank_time_us);            // Let ghosting logic work
            multiplexer->SetRow(counter);               // Change the row
            counter = (counter + 1) % cfg.cfg.multiplex;
        }
    }
    
    delete multiplexer;
    shutdown = false;
}

bool getCFG(CFG_t *cfg) {
    uint32_t counter = 0;
    char *token;
    char str[50];
    
    get_string(str, sizeof(str));
    token = strtok(str, ",");
    while(token != NULL) {
        token = strtok(NULL, ",");
        ++counter;
    }
    
    if (counter != 7) {
        printf("Error: Bad String\n");
        return true;
    }
    
    token = strtok(str, ",");
    while(token != NULL) {
        cfg->v[counter] = atoi(token);
        token = strtok(NULL, ",");
        ++counter;
    }
    
    return false;
}

