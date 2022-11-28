// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Copyright (C) 2013 Henner Zeller <h.zeller@acm.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://gnu.org/licenses/gpl-2.0.txt>

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

// Raspberry 1 and 2 have different base addresses for the periphery
#define BCM2708_PERI_BASE        0x20000000
#define BCM2709_PERI_BASE        0x3F000000
#define BCM2711_PERI_BASE        0xFE000000

#define GPIO_REGISTER_OFFSET         0x200000

#define GPIO_PWM_BASE_OFFSET	(GPIO_REGISTER_OFFSET + 0xC000)
#define GPIO_CLK_BASE_OFFSET	0x101000

#define REGISTER_BLOCK_SIZE (4*1024)

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

#define GPIO_BIT(x) (1ull << x)

GPIO::GPIO() : output_bits_(0), input_bits_(0), reserved_bits_(0),
               slowdown_(1)
#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
             , uses_64_bit_(false)
#endif
{
}

gpio_bits_t GPIO::InitOutputs(gpio_bits_t outputs) {
  if (s_GPIO_registers == NULL) {
    fprintf(stderr, "Attempt to init outputs but not yet Init()-ialized.\n");
    return 0;
  }

  outputs &= ~(output_bits_ | input_bits_ | reserved_bits_);

#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
  const int kMaxAvailableBit = 45;
  uses_64_bit_ |= (outputs >> 32) != 0;
#else
  const int kMaxAvailableBit = 31;
#endif
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
#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
  const int kMaxAvailableBit = 45;
  uses_64_bit_ |= (inputs >> 32) != 0;
#else
  const int kMaxAvailableBit = 31;
#endif
  for (int b = 0; b <= kMaxAvailableBit; ++b) {
    if (inputs & GPIO_BIT(b)) {
      INP_GPIO(b);
    }
  }
  input_bits_ |= inputs;
  return inputs;
}

// We are not interested in the _exact_ model, just good enough to determine
// What to do.
enum RaspberryPiModel {
  PI_MODEL_1,
  PI_MODEL_2,
  PI_MODEL_3,
  PI_MODEL_4
};

static int ReadFileToBuffer(char *buffer, size_t size, const char *filename) {
  buffer[0] = '\0';
  const int fd = open(filename, O_RDONLY);
  if (fd < 0) return -1;
  ssize_t r = read(fd, buffer, size - 1); // assume one read enough
  buffer[r >= 0 ? r : 0] = '\0';
  close(fd);
  return r;
}

static RaspberryPiModel DetermineRaspberryModel() {
  char buffer[4096];
  if (ReadFileToBuffer(buffer, sizeof(buffer), "/proc/cpuinfo") < 0) {
    fprintf(stderr, "Reading cpuinfo: Could not determine Pi model\n");
    return PI_MODEL_3;  // safe guess fallback.
  }
  static const char RevisionTag[] = "Revision";
  const char *revision_key;
  if ((revision_key = strstr(buffer, RevisionTag)) == NULL) {
    fprintf(stderr, "non-existent Revision: Could not determine Pi model\n");
    return PI_MODEL_3;
  }
  unsigned int pi_revision;
  if (sscanf(index(revision_key, ':') + 1, "%x", &pi_revision) != 1) {
    fprintf(stderr, "Unknown Revision: Could not determine Pi model\n");
    return PI_MODEL_3;
  }

  // https://www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
  const unsigned pi_type = (pi_revision >> 4) & 0xff;
  switch (pi_type) {
  case 0x00: /* A */
  case 0x01: /* B, Compute Module 1 */
  case 0x02: /* A+ */
  case 0x03: /* B+ */
  case 0x05: /* Alpha ?*/
  case 0x06: /* Compute Module1 */
  case 0x09: /* Zero */
  case 0x0c: /* Zero W */
    return PI_MODEL_1;

  case 0x04:  /* Pi 2 */
  case 0x12:  /* Zero W 2 (behaves close to Pi 2) */
    return PI_MODEL_2;

  case 0x11: /* Pi 4 */
  case 0x13: /* Pi 400 */
  case 0x14: /* CM4 */
    return PI_MODEL_4;

  default:  /* a bunch of versions representing Pi 3 */
    return PI_MODEL_3;
  }
}

static RaspberryPiModel GetPiModel() {
  static RaspberryPiModel pi_model = DetermineRaspberryModel();
  return pi_model;
}

static int GetNumCores() {
  return GetPiModel() == PI_MODEL_1 ? 1 : 4;
}

static uint32_t *mmap_bcm_register(off_t register_offset) {
  off_t base = BCM2709_PERI_BASE;  // safe fallback guess.
  switch (GetPiModel()) {
  case PI_MODEL_1: base = BCM2708_PERI_BASE; break;
  case PI_MODEL_2: base = BCM2709_PERI_BASE; break;
  case PI_MODEL_3: base = BCM2709_PERI_BASE; break;
  case PI_MODEL_4: base = BCM2711_PERI_BASE; break;
  }

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
    fprintf(stderr, "MMapping from base 0x%lx, offset 0x%lx\n",
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

bool GPIO::Init(int slowdown) {
  slowdown_ = slowdown;

  // Pre-mmap all bcm registers we need now and possibly in the future, as to
  // allow  dropping privileges after GPIO::Init() even as some of these
  // registers might be needed later.
  if (!mmap_all_bcm_registers_once())
    return false;

  gpio_set_bits_low_ = s_GPIO_registers + (0x1C / sizeof(uint32_t));
  gpio_clr_bits_low_ = s_GPIO_registers + (0x28 / sizeof(uint32_t));
  gpio_read_bits_low_ = s_GPIO_registers + (0x34 / sizeof(uint32_t));

#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
  gpio_set_bits_high_ = s_GPIO_registers + (0x20 / sizeof(uint32_t));
  gpio_clr_bits_high_ = s_GPIO_registers + (0x2C / sizeof(uint32_t));
  gpio_read_bits_high_ = s_GPIO_registers + (0x38 / sizeof(uint32_t));
#endif

  return true;
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
