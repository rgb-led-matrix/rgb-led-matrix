# Differences from upstream
I decided that I would not use the Raspberry Pi GPIO implementation moving forward. While this is likley possible it is not worth the effort to support. Microcontroller suchas the RP2040 provide means of support without much of the mess, however does increase the cost. The original code base had trouble scaling because of the number of pins. RP2040 is only capable of scaling due to RAM, IO bandwidth/latency, pins and processing power. Using multiple RP2040s is required for scaling.

Issues found within the original code (most could be fixed or mitigated):
* Bit banging is time critical and blocking operations will cause issues for stability
* GPIO accesses can be blocking and/or non-deterministic if bus arbitration or contention occurs
  * Library owns the GPIO register(s)
  * Accesses from other cores to IO address space has unknown effect
* Memory accesses can be blocking and/or non-deterministic if bus arbitration or contention occurs
* Multicore is required for preventing the operating system from blocking LED multiplexing
* Multicore can be used to manage cache and fight memory access issues but this could reduce useful core count from 3 to 2
  * Prefetch was not found to be capable of mitigating this
  * May require L3/L4 block RAM to work properly
  * However it is probably best to lower the HUB75 clock rate to allow memory operations to be less critical
    * Memory operations from cache are believed to be capable of high HUB75 clock rates but only for smaller panels using low color depth or BCM.
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
* Shift length and indirectly pixel density are limited by clock fanout
  * Extending this beyond the fanout limit requires a repeater, which is not common or useful except for panels with LED drivers using RAM buffers

The approach used in the original code base is in fact possible. However it somewhat champions a different hardware architecture or use of the Raspberry Pi as a hardware module. Using the original code base as a library in generic applications is not completely recommended. Again certain hardware and/or software mitigations are required. Conceptual portability is maintained in spite of this.

Major benefit of the original code base is low SRAM requirement. LED Panels are mostly sequential elements. With high bandwidth DRAM, only about 2 shift registers lengths are required to be stored in SRAM using ping-pong. Memory represents one of the largest costs for LED Matrix. Computation is fairly easy to accelerate with FPGAs however the memory bandwidth requirements can be extreme using this approach. Moving to cheap DRAM with higher density can actually reduce the bandwidth requirements and possibly the computation requirements. While you can save on the density with brute force computation you may end up paying more in power consumption or IO bus width. Cost of DRAM allows high density with high bandwidth using SRAM as FIFO to maintain the pipeline used to multiplex the LED panel.

Different hardware architectures exist which support the original code base's intention, however these may be more expensive than the Raspberry Pi. This is slowly changing. Eventually this concept will likely work without issue. Cortex-A support for non-blocking GPIO access has yet to be proven in many of them, as this is usually handled by Cortex-R/Cortex-M. Alternatively, high bandwidth narrow random writes to self paced/clocked hardware would remove this requirement. (It has been suggested that Raspberry Pi favors high bandwidth wide sequential writes.)
