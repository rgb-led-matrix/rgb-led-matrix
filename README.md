# rgb-led-matrix
This repository creates a library for top level applications and graphics. This repository will be a sub-module to that repository. The purpose of this code base is to allow better support for different small form factor boards regardless of hardware, operating system, etc. It also allows for support of different implementations.

Currently all bit-bang implementations have been removed. Only external implementations are used. This may change, however these should enable better performance and portability, despite possibly being more expensive.

## Status
Work in progress. Code is licensed under GPL 2. The original code base was https://github.com/hzeller/rpi-rgb-led-matrix. This code base has diverged from this code base signifcantly, I went another direction with it. Note my changes are also protected under GPL 2.

There is no warranty for this. Work is free to use according to the [GPL 2 license](COPYING). I am not responsible or liable for any damages or issues as a result of this in any way. User is responsible for all laws and regulations as required. Work is AS-IS and it is the respiblity of any user to address any shortages discovered in this code base before using it.

## Building
Note this is a hack for now. Currently it builds both the Raspberry Pi and Rock Pi S.

```bash
bash build.sh
```

### Raspberry Pi (Requires /dev/gpiomem, available by default on Raspberry Pi Kernel)
#### rgb-led-matrix (Required)
```bash
bash scripts/pi_cross_compile_setup.sh  # Only the first time
bash build.sh # TODO: Make build Raspberry Pi only
```
#### rgb-led-matrix-rp2040-module (Required for HUB75, unless using some other rgb-led-matrix-module. Requires multicore. (Pi 2, Pi 3, Pi 4, Pi Zero 2, etc.))
Edit configuration and program:
```bash
cd firmware/PICO/rgb-led-matrix-rp2040-module
vim RP2040_Module/config.h
bash setup.sh   # Only the first time
bash build.sh
# TODO: Call bootloader script
cd ../../..
```

#### LED_Matrix_RP2040 (Required for external/RP2040. May want multicore, not required. (Pi 2, Pi 3, Pi 4, Pi Zero 2, etc.))
Edit configuration and program:
```bash
cd external/RP2040/LED_Matrix_RP2040
git submodule update --init
vim ../cfg.xml
groovy build.groovy -c ../cfg.xml
./LED_Matrix/build/build.sh
# TODO: Call bootloader script
cd ../../../
```

### Rock Pi S
#### /dev/gpiomem kernel module (Required)
Cross compilation recommended, note this currently downloads and builds the linux kernel.
```bash
cd modules/rock-pi-s/rock_pi_s_gpiomem
bash setup.sh   # Only the first time
bash build_cross.sh
sudo insmod gpiomem.ko
cd ../../..
```
Note if you need to use different kernel version or kernel configuration. You will need to change the setup script.

To change the kernel version:
```bash
cd kernel
git checkout <branch/tag>

# If cross compiling, export these:
export ARCH=arm64
export CROSS_COMPILE=$PWD/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

make modules_prepare
cd ..
make
```

To change the kernel configuration:
```bash
cd kernel

# If cross compiling, export these:
export ARCH=arm64
export CROSS_COMPILE=$PWD/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

# If you do not have default configuration
make menuconfig

# If you do have default configuration
make <default_config>

make modules_prepare
cd ..
make
```

#### rgb-led-matrix (Required)
```bash
bash scripts/rock_pi_s_cross_compile_setup.sh  # Only the first time
bash build.sh # TODO: Make build Rock Pi S only
```
#### rgb-led-matrix-rp2040-module (Required for HUB75, unless using some other rgb-led-matrix-module)
Edit configuration and program:
```bash
cd firmware/PICO/rgb-led-matrix-rp2040-module
vim RP2040_Module/config.h
bash setup.sh   # Only the first time
bash build.sh
# TODO: Call bootloader script
cd ../../..
```

#### LED_Matrix_RP2040 (Required for external/RP2040)
Edit configuration and program:
```bash
cd external/RP2040/LED_Matrix_RP2040
git submodule update --init
vim ../cfg.xml
groovy build.groovy -c ../cfg.xml
./LED_Matrix/build/build.sh
# TODO: Call bootloader script
cd ../../../
```

## Layout
There are six sections at this directory level. Look at lower levels for more details about each section.

### Hardware
This is a section for hardware adapter modules. Currently there are two classes.

HUB75 - These are for the HUB75 framebuffer implementations. These are bit bang notions and do not use external. These may require a kernel module in modules, depending on platform.

external - These are for the external framebuffer implementations. See external section. Currently the only subclass is RP2040, which is based on LED_Matrix_RP2040.

### external
This is a section for hardware offload modules used in rgb-led-matrix/lib/src/framebuffer. Some external modules may have better features.

### firmware
This section is for the pin-pulser implementation, which could be moved to an external microcontroller. (This may be dropped.) This was done to improve performance and portability. Firmware submodules used here are derived from rgb-led-matrix-module. Currently the only implementation is rgb-led-matrix-rp2040-module using the RPi PICO module.

### modules
This section is for kernel modules used by different platforms. These kernel modules provide things like user space access to GPIO registers with standard priviledges. Currently the only implementation is rock_pi_s_gpiomem which creates /dev/mem on Rock Pi S using 4.4 Linux Kernel. However this logic should be portable to other platforms and kernel versions, using Linux.

### rgb-led-matrix
This is the main library source directory. The top level library includes are in rgb-led-matrix/include while the internal source is divided into rgb-led-matrix/lib which has an include and src folder. Applications do not need to worry about the internal logic, just the top level includes and the libraries.

### scripts
This is a folder containing helper scripts for setup and other things. These will be referenced by other sections.
