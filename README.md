# rgb-led-matrix
This repository creates a library for top level applications and graphics. This repository will be a sub-module to that repository. The purpose of this code base is to allow better support for different small form factor boards regardless of hardware, operating system, etc. It also allows for support of different implementations.

Currently all bit-bang implementations have been removed. Only external implementations are used. This may change, however these should enable better performance and portability, despite possibly being more expensive.

## Status
Work in progress. Code is licensed under GPL 2. The original code base was https://github.com/hzeller/rpi-rgb-led-matrix. This code base has diverged from this code base signifcantly, I went another direction with it. Note my changes are also protected under GPL 2.

There is no warranty for this. Work is free to use according to the [GPL 2 license](COPYING). I am not responsible or liable for any damages or issues as a result of this in any way. User is responsible for all laws and regulations as required. Work is AS-IS and it is the respiblity of any user to address any shortages discovered in this code base before using it.

## Differences from fork
I decided that I would not use the Raspberry Pi GPIO implementation moving forward. While this is likley possible it is not worth the effort to support. Microcontroller suchas the RP2040 provide means of support without much of the mess, however does increase the cost. The original code base had trouble scaling because of the number of pins. RP2040 is only capable of scaling due to RAM, IO bandwidth/latency, pins and processing power. Using multiple RP2040s is required for scaling.

Issues found within the original code (most could be fixed or mitigated):
* Bit banging is time critical and blocking operations will cause issues for stability
* GPIO accesses can be blocking and/or non-deterministic if bus arbitration or contention occurs
  * Library owns the GPIO register(s)
  * Accesses from other cores to IO address space has unknown effect
* Memory accesses can be blocking and/or non-deterministic if bus arbitration or contention occurs
* Multicore is required for preventing the operating system from blocking LED multiplexing
* Multicore can be used to manage cache and fight memory access issues but this could reduce useful core count from 3 to 2
* Bit plane timing is easy to misconfigure, which could be resolved in runtime potentally
* High refresh rate is not supported as it does not account for ghosting due to small capacitances built into the display
  * This likely can be address with a uS timer or tight assembly
* Cache locality management plays a large role in stability, assuming multicore locality management is not used
* Understanding the max number of bitplanes allowed is not documented
  * Difference situations exist: multiplex ratio, distance, dot correction, power level, light polution, etc.
* BCM has a small effect on the accuracy due to the number of edges within a PWM period
* Documentation of interference, signal stability (transmission lines), power supply response, etc. is not present
* Misconfiguration of timing will effect accuracy, which is generally more common in larger displays
* kBitPlanes is actually the recommended way to configure the number of PWM bits at compile time rather than run time
* BCM is used for reducing computational latency of bit plane generation for frames however this may still be an issue for some applications desiring a high frame rate

## Building
Note this is a hack for now. Currently it builds both the Raspberry Pi and Rock Pi S.

```bash
bash build.sh
```

## Layout
There are three sections at this directory level. Look at lower levels for more details about each section.

### external
This is a section for hardware offload modules used in rgb-led-matrix/lib/src/framebuffer. Some external modules may have better features.

### rgb-led-matrix
This is the main library source directory. The top level library includes are in rgb-led-matrix/include while the internal source is divided into rgb-led-matrix/lib which has an include and src folder. Applications do not need to worry about the internal logic, just the top level includes and the libraries.

### scripts
This is a folder containing helper scripts for setup and other things. These will be referenced by other sections.
