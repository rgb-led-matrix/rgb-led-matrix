#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "port/gpio/gpio.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

// TODO:
#define PERI_BASE                 0x20000000
#define GPIO_REGISTER_OFFSET      0x200000
#define REGISTER_BLOCK_SIZE       (4*1024)

// TODO:
// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x).
#define INP_GPIO(g) *(s_GPIO_registers+((g)/10)) &= ~(7ull<<(((g)%10)*3))
#define OUT_GPIO(g) *(s_GPIO_registers+((g)/10)) |=  (1ull<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

// We're pre-mapping all the registers on first call of GPIO::Init(),
// so that it is possible to drop privileges afterwards and still have these
// usable.
static volatile uint32_t *s_GPIO_registers = NULL;

namespace rgb_matrix {

// TODO:
#define GPIO_BIT(x) (1ull << x)

GPIO::GPIO() : output_bits_(0), input_bits_(0), reserved_bits_(0) {
}

gpio_bits_t GPIO::InitOutputs(gpio_bits_t outputs) {
  if (s_GPIO_registers == NULL) {
    fprintf(stderr, "Attempt to init outputs but not yet Init()-ialized.\n");
    return 0;
  }

  outputs &= ~(output_bits_ | input_bits_ | reserved_bits_);

  const int kMaxAvailableBit = 31;
  for (int b = 0; b <= kMaxAvailableBit; ++b) {
    if (outputs & GPIO_BIT(b)) {
      INP_GPIO(b);   // for writing, we first need to set as input.
      OUT_GPIO(b);
    }
  }
  output_bits_ |= outputs;
  return outputs;
}

gpio_bits_t GPIO::RequestInputs(gpio_bits_t inputs) {
  if (s_GPIO_registers == NULL) {
    fprintf(stderr, "Attempt to init inputs but not yet Init()-ialized.\n");
    return 0;
  }

  inputs &= ~(output_bits_ | input_bits_ | reserved_bits_);
  const int kMaxAvailableBit = 31;
  for (int b = 0; b <= kMaxAvailableBit; ++b) {
    if (inputs & GPIO_BIT(b)) {
      INP_GPIO(b);
    }
  }
  input_bits_ |= inputs;
  return inputs;
}

static uint32_t *mmap_bcm_register(off_t register_offset) {
  off_t base = PERI_BASE;

  int mem_fd;
  if ((mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC) ) < 0) {
    return NULL;
  }

  uint32_t *result =
    (uint32_t*) mmap(NULL,                  // Any adddress in our space will do
                     REGISTER_BLOCK_SIZE,   // Map length
                     PROT_READ|PROT_WRITE,  // Enable r/w on GPIO registers.
                     MAP_SHARED,
                     mem_fd,                // File to map
                     base + register_offset // Offset to bcm register
                     );
  close(mem_fd);

  if (result == MAP_FAILED) {
    perror("mmap error: ");
    fprintf(stderr, "Mapping from base 0x%lx, offset 0x%lx\n",
            base, register_offset);
    return NULL;
  }
  return result;
}

static bool mmap_all_bcm_registers_once() {
  if (s_GPIO_registers != NULL) return true;  // alrady done.

  // The common GPIO registers.
  s_GPIO_registers = mmap_bcm_register(GPIO_REGISTER_OFFSET);
  if (s_GPIO_registers == NULL) {
    return false;
  }

  return true;
}

bool GPIO::Init() {
  // Pre-mmap all bcm registers we need now and possibly in the future, as to
  // allow  dropping privileges after GPIO::Init() even as some of these
  // registers might be needed later.
  if (!mmap_all_bcm_registers_once())
    return false;

  // TODO:
  //gpio_set_bits_low_ = s_GPIO_registers + (0x1C / sizeof(uint32_t));
  //gpio_clr_bits_low_ = s_GPIO_registers + (0x28 / sizeof(uint32_t));
  //gpio_read_bits_low_ = s_GPIO_registers + (0x34 / sizeof(uint32_t));

  return false;
}

// For external use, e.g. in the matrix for extra time.
uint32_t GetMicrosecondCounter() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  const uint64_t micros = ts.tv_nsec / 1000;
  const uint64_t epoch_usec = (uint64_t)ts.tv_sec * 1000000 + micros;
  return epoch_usec & 0xFFFFFFFF;
}

} // namespace rgb_matrix
