TODO

The current structure of the code is divided into three sections.

#### Framebuffer
At the bottom you have the interfaces and controllers these interact with the actual LED panels. This code base is only allowed to use memory, GPIO and networking as the interface. Anything requiring tight timing cannot be handled in one of those interfaces must use external logic. The software interface for these is Framebuffer. 

Different external framebuffers have been conceptualized for other projects.
- GPIO Bit-bang
- Medium to large FPGA via bit-bang (Memory can be internal or external)
- Zynq
- RP2040 as hardware module
- Microcontroller and small FPGA
- Using receiver card (I am still checking to see if this is allowed.)

#### Port
The middle layer is the hardware platform. These allow porting the code to Linux, Windows IoT, etc. It also allows supporting Raspberry Pi, BeagleBone Black, Rock Pi S, STM32MP1, Zynq, etc. The pin-pulser could be internal for BeagleBone Black, STM32MP1, Zynq, etc. Again requirements are GPIO, L2/L3 networking and memory interfaces in the case of Zynq. 

Pin mapper is used to convert GPIO signals, Ethernet interface names, etc. At some point pin-mapping will be abstracted into a better interface. 

Pin-pulser part of this, which for the time being is handled externally via a microcontroller. The RPI PICO is used for simplicity however adding support for something like PIC32 using FreeRTOS should be possible. This uses a UART connection to send configuration updates and GPIO to interact with the bit-bang libraries. This will support OE and GCLK based panels.

Pin-pulser could be implemented using internal logic. However this would represent a different port API.

No bare metal support is planned, however in some cases this could be possible. User space logic is preferred. No support for RTLinux, Kernel Thread/Modules, etc. is planned. (No /dev/mem black magic.)

These will be compiled into libraries containing the required interface implementations. These will be used by the application logic to interface with the hardware. There will be one per hardware port.

#### Application
The top layer is the applications and graphics. These use the libraries created from the lower two layers. This repository will be a sub-module to that repository.
